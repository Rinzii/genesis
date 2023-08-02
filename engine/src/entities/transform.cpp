#include <entities/transform.hpp>

namespace gen
{
	void Transform::setWorldRotation(const mim::quat<float> & newRot)
	{
		const Transform* parent = m_parent;
		mim::quat<float> worldRot;

		while (parent)
		{
			worldRot = parent->getLocalRotation() * worldRot;
			parent = parent->m_parent;
		}

		m_localRot = worldRot * newRot;
		m_dirty = true;
	}

	void Transform::setWorldRotation(const mim::vec3<float> & newRot)
	{
		const Transform* parent = m_parent;
		mim::quat<float> worldRot;

		while (parent)
		{
			worldRot = parent->getLocalRotation() * worldRot;
			parent = parent->m_parent;
		}

		m_localRot = worldRot * newRot;
		m_dirty = true;
	}

	void Transform::setWorldPosition(const mim::vec3<float> & newPos)
	{
		const Transform* parent = m_parent;
		mim::vec3<float> pos;

		while (parent)
		{
			pos += parent->getLocalPosition();
			parent = parent->m_parent;
		}

		m_localScl = newPos - pos;
		m_dirty = true;
	}

	void Transform::setWorldScale(const mim::vec3<float> & newScale)
	{
		const Transform* parent = m_parent;
		mim::vec3<float> scl{1};

		while (parent)
		{
			scl *= parent->getLocalScale();
			parent = parent->m_parent;
		}

		m_localScl = newScale / scl;
		m_dirty = true;
	}

	void Transform::setLocalRotation(const mim::quat<float> & newRot)
	{
		m_localRot = newRot;
		m_dirty = true;
	}

	void Transform::setLocalRotation(const mim::vec3<float> & newRot)
	{
		m_localRot = mim::quat{ newRot };
		m_dirty = true;
	}

	void Transform::setLocalPosition(const mim::vec3<float> & newPos)
	{
		m_localPos = newPos;
		m_dirty = true;
	}

	void Transform::setLocalScale(const mim::vec3<float> & newScl)
	{
		m_localScl = newScl;
		m_dirty = true;
	}

	mim::quat<float> Transform::getWorldRotation() const
	{
		const Transform* parent = m_parent;
		mim::quat<float> worldRot = m_localRot;

		while (parent)
		{
			worldRot = parent->getLocalRotation() * worldRot;
			parent = parent->m_parent;
		}

		return worldRot;
	}

	mim::vec3<float> Transform::getWorldPosition() const
	{
		const Transform* parent = m_parent;
		mim::vec3<float> worldPos = m_localPos;

		while (parent)
		{
			worldPos += parent->getLocalPosition();
			parent = parent->m_parent;
		}

		return worldPos;
	}

	mim::vec3<float> Transform::getWorldScale() const
	{
		const Transform* parent = m_parent;
		mim::vec3<float> worldScl = m_localScl;

		while (parent)
		{
			worldScl *= parent->getLocalScale();
			parent = parent->m_parent;
		}

		return worldScl;
	}

	mim::quat<float> Transform::getLocalRotation() const
	{
		return m_localRot;
	}

	mim::vec3<float> Transform::getLocalPosition() const
	{
		return m_localPos;
	}

	mim::vec3<float> Transform::getLocalScale() const
	{
		return m_localScl;
	}
}
