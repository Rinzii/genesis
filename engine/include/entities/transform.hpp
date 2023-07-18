#pragma once
#include <mim/vec3.hpp>
#include <mim/quat.hpp>
#include <mim/mat4.hpp>
#include "base/config/compilerTraits.hpp"

namespace gen
{
	class Transform //All the world functions calculations have been determined using a drawing on paint, they will be tested later TODO: Test world setters and getters
	{
	public:

		void setWorldRotation(mim::quat<float>& newRot);
		void setWorldRotation(mim::vec3<float>& newRot);
		void setWorldPosition(const mim::vec3<float>& newPos);
		void setWorldScale(const mim::vec3<float>& newScale);

		void setLocalRotation(const mim::quat<float>& newRot);
		void setLocalRotation(const mim::vec3<float>& newRot);
		void setLocalPosition(const mim::vec3<float>& newPos);
		void setLocalScale(const mim::vec3<float>& newScl);

		//GEN_NODISCARD mim::mat4<float> getWorldMatrix() const;

		GEN_NODISCARD mim::quat<float> getWorldRotation() const;
		GEN_NODISCARD mim::vec3<float> getWorldPosition() const;
		GEN_NODISCARD mim::vec3<float> getWorldScale() const;

		GEN_NODISCARD mim::quat<float> getLocalRotation() const;
		GEN_NODISCARD mim::vec3<float> getLocalPosition() const;
		GEN_NODISCARD mim::vec3<float> getLocalScale() const;

	private:

		Transform* m_parent = nullptr;

		mim::quat<float> m_localRot;
		mim::vec3<float> m_localPos;
		mim::vec3<float> m_localScl;

		bool m_dirty = false;
	};
}
