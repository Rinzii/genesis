// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <memory>
#include <stdexcept>
#include "config.hpp"
#include "sink.hpp"
#include "target.hpp"

namespace gen::logger
{
	///
	/// \brief Logger Instance: a single instance must be created within main's scope.
	///
	class Instance
	{
	public:
		///
		/// \brief Error thrown if more than one instance is attempted to be created.
		///
		struct DuplicateError : std::runtime_error
		{
			using std::runtime_error::runtime_error;
		};

		Instance(Instance &&)			  = delete;
		Instance & operator=(Instance &&) = delete;

		Instance(Instance const &)			   = delete;
		Instance & operator=(Instance const &) = delete;

		///
		/// \brief Create a logger Instance.
		/// \param filePath path to create/overwrite log file at.
		/// \param config Config to use.
		///
		explicit Instance(char const * filePath = "genesis.log", Config config = {});
		~Instance();

		///
		/// \brief Obtain a copy of the Config in use.
		///
		[[nodiscard]] Config getConfig() const;

		///
		/// \brief Overwrite the Config in use.
		///
		void setConfig(Config config);

		///
		/// \brief Add a custom sink.
		///
		void addSink(std::unique_ptr<Sink> sink);

		///
		/// \brief Entrypoint for logging (free) functions.
		///
		static void print(std::string_view message, Context const & context);

	private:
		struct Impl;
		struct Deleter
		{
			void operator()(Impl const * ptr) const;
		};

		// NOLINTNEXTLINE
		inline static Impl * s_instance{};
		std::unique_ptr<Impl, Deleter> m_impl{};
	};
} // namespace gen::logger
