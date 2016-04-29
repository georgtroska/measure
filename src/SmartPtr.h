// $Id$

// Copyright (C) 2003-2008 Oliver Schulz <oliver.schulz@tu-dortmund.de>

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#ifndef SmartPtr_h
#define SmartPtr_h


namespace dce {


/// @brief  Similar to std::auto_ptr, but can manage multiple references.
/// @param  Type  The type to point to.
///
/// SmartPtr uses a reference counter. Therefore more than one can point
/// to the same object. Assignment of a SmartPrt to another does not invalidate
/// the original SmartPtr. This is the main difference to std::auto_ptr.
/// When the last SmartPtr pointing to an object is destroyed, the object
/// is deleted.

template<class Type> class SmartPtr {
protected:

	Type* m_trg;
	int* m_refs;

	void init(Type *obj) {
		m_trg = obj;
		m_refs = new int(1);
	}

	template<class CompType> void init(const SmartPtr<CompType> &ptr) {
		if (ptr.valid()) {
			m_trg = ptr.m_trg;
			m_refs = ptr.m_refs;
			(*m_refs)++;
		} else init(0);
	}
	
	void clear() {
		if (*m_refs > 1) (*m_refs)--;
		else {
			if (m_trg!=0) delete m_trg;
			delete m_refs;
		}
	}

	template<class CompType> friend class SmartPtr;

public:

	/// @brief  Check if this SmartPtr is valid (doesn't point to 0).
	/// @return @c tue if valid, @false if not.
	bool valid() const { return m_trg!=0; }

	/// @brief  Get target of this SmartPtr.
	/// @return A normal pointer to the target.
	Type* target() const { return m_trg; }

	/// @brief Get the reference count.
	/// @return The number of SmartPtr instances pointing to target().
	int references() const { return *m_refs; }

	/// @brief  Pointer dereferencing operator.
	/// @return The target object.
	Type& operator*() const { return *m_trg; }

	/// @brief  Arrow-Style pointer dereferencing operator.
	/// @return The pointer itself, which the language then will
	///         automatically cause to be dereferenced.
	Type* operator->() const { return m_trg; }

	/// @brief  Type conversion to normal pointer.
	operator Type*() const { return m_trg; }

	/// @brief  Assingment of SmartPtr of the same type.
	/// @return @c *this
	///
	/// This SmartPtr is set up to point to the target of @p ptr.
	/// Reference count of new target is incremented,
	/// reference count of old target is decremented. Old target
	/// is deleted if this was the last reference to it.
	SmartPtr<Type>& operator=(const SmartPtr<Type> ptr) {
		clear(); init(ptr);
		return *this;
	}

	/// @brief  Assingment of SmartPtr of different but compatible type.
	/// @return @c *this
	///
	/// This SmartPtr is set up to point to the target of @p ptr.
	/// Reference count of new target is incremented,
	/// reference count of old target is decremented. Old target
	/// is deleted if this was the last reference to it.
	template<class CompType> SmartPtr<Type>& operator=(const SmartPtr<CompType> ptr) {
		clear(); init(ptr);
		return *this;
	}

	/// @brief  SmartPtr comparison.
	/// @return @c true if SmartPtr targets are identical, @c false if not. 
	template<class CompType> bool operator==(const SmartPtr<CompType> ptr) const
		{ return m_trg==ptr.m_trg; }

	/// @brief  SmartPtr - standard pointer comparison.
	/// @return @c true if targets are the identical, @c false if not. 
	template<class CompType> bool operator==(const CompType* ptr) const
		{ return m_trg==ptr; }


	/// @brief Construct invalid SmartPtr, pointing to 0.
	SmartPtr() { init(0); }

	/// @brief Construct from reference.
	/// @param obj  The intended target for this SmartPtr.
	///
	/// SmartPtr is set up to point to @p obj. Note that obj may
	/// be deleted at destruction of this SmartPtr.
	SmartPtr(Type &obj) { init(&obj); }

	/// @brief Construct from pointer.
	/// @param obj  Pointer to the intended target for this SmartPtr.
	///
	/// SmartPtr is set up to point to @p obj. Note that obj may
	/// be deleted at destruction of this SmartPtr.
	SmartPtr(Type *obj) { init(obj); }

	/// @brief Copy constructor
	/// @see   operator=()
	SmartPtr(const SmartPtr<Type> &ptr) { init(ptr); }

	/// @brief Copy constructor
	/// @see   operator=()
	template<class CompType> SmartPtr(const SmartPtr<CompType> &ptr) { init(ptr); }

	/// @brief Destructor
	///
	/// Reference count of target is decremented. Target
	/// is deleted if this SmartPtr the last reference to it.
	virtual ~SmartPtr() { clear(); }
};


} // namespace dce

#endif // SmartPtr_h
