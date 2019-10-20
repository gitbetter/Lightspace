#pragma once

#include "tensor.hpp"

namespace ls {
	class shape
	{
	public:

		shape() :
			origin_( f_point( 0, 0, 0 ) ), id_( get_uid() )
		{ }
		shape( const f_point& o ) :
			origin_( o ), id_( get_uid() )
		{ }
		virtual ~shape() { }

		const f_point& origin() const noexcept
		{
			return origin_;
		}

		bool operator==( const shape& rhs ) const noexcept
		{
			return id_ == rhs.id_;
		}

	protected:

		uint32_t id_;
		f_point origin_;

	};

	class sphere : public shape
	{
	public:

		sphere() : shape(), 
			radius_( 1 )
		{ }
		sphere( const f_point& o, fpnum r ) : shape( o ),
			radius_( r )
		{ }

		const fpnum radius() const noexcept
		{
			return radius_;
		}

		bool operator=( const shape& rhs ) const noexcept = delete;

	private:

		fpnum radius_;

	};
}