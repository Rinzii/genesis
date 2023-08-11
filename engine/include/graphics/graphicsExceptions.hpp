// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once


class VulkanException : public std::exception {
public:

	/**
	 * Vulkan specific exception that also outputs the exception message to the genesis logger.
	 * @param message exception message
	 */
	explicit VulkanException(const char* message);

	[[nodiscard]] const char* what() const noexcept override;

private:
	std::string m_message;
};


