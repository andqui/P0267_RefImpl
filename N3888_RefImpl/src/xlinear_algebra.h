#pragma once

namespace std::experimental::io2d {
	inline namespace v1 {

		class point_2d {
		public:
			// \ref{\iotwod.\pointtwod.cons}, constructors:
			constexpr point_2d() noexcept;
			constexpr point_2d(float x, float y) noexcept;

			float x;
			float y;

			// \ref{\iotwod.\pointtwod.observers}, observers:
			constexpr float dot(const point_2d& other) const noexcept;
			float magnitude() const noexcept;
			constexpr float magnitude_squared() const noexcept;
			float angular_direction() const noexcept;
			point_2d to_unit() const noexcept;
			constexpr static point_2d zero() noexcept {
				return { 0.0f, 0.0f };
			}

			// \ref{\iotwod.\pointtwod.member.ops}, member operators:
			constexpr point_2d& operator+=(const point_2d& rhs) noexcept;
			constexpr point_2d& operator-=(const point_2d& rhs) noexcept;
			constexpr point_2d& operator*=(float rhs) noexcept;
			constexpr point_2d& operator*=(const point_2d& rhs) noexcept;
			constexpr point_2d& operator/=(float rhs) noexcept;
			constexpr point_2d& operator/=(const point_2d& rhs) noexcept;
		};
		constexpr bool operator==(const point_2d& lhs, const point_2d& rhs)
			noexcept;
		constexpr bool operator!=(const point_2d& lhs, const point_2d& rhs)
			noexcept;
		constexpr point_2d operator+(const point_2d& lhs) noexcept;
		constexpr point_2d operator+(const point_2d& lhs, const point_2d& rhs)
			noexcept;
		constexpr point_2d operator-(const point_2d& lhs) noexcept;
		constexpr point_2d operator-(const point_2d& lhs, const point_2d& rhs)
			noexcept;
		constexpr point_2d operator*(const point_2d& lhs, float rhs) noexcept;
		constexpr point_2d operator*(float lhs, const point_2d& rhs) noexcept;

		class matrix_2d {
		public:
			constexpr matrix_2d() noexcept
				: matrix_2d(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f) {}
			constexpr matrix_2d(float v00, float v01, float v10, float v11, float v20, float v21) noexcept
				: m00(v00)
				, m01(v01)
				, m02(0.0f)
				, m10(v10)
				, m11(v11)
				, m12(0.0f)
				, m20(v20)
				, m21(v21)
				, m22(1.0f) {
			}

			float m00;
			float m01;
			float m02;
			float m10;
			float m11;
			float m12;
			float m20;
			float m21;
			float m22;

			// \ref{\iotwod.\matrixtwod.staticfactories}, static factory functions:
			constexpr static matrix_2d init_translate(const point_2d& value) noexcept {
				return{ 1.0F, 0.0F, 0.0F, 1.0F, value.x, value.y };
			}
			constexpr static matrix_2d init_scale(const point_2d& value) noexcept {
				return{ value.x, 0.0F, 0.0F, value.y, 0.0F, 0.0F };
			}
			static matrix_2d init_rotate(float radians) noexcept {
				auto sine = sin(radians);
				auto cosine = cos(radians);
				sine = _Round_floating_point_to_zero(sine);
				cosine = _Round_floating_point_to_zero(cosine);
				return{ cosine, -sine, sine, cosine, 0.0F, 0.0F };
			}
			static matrix_2d init_reflect(float radians) noexcept {
				auto sine = sin(radians * 2.0F);
				auto cosine = cos(radians * 2.0F);
				sine = _Round_floating_point_to_zero(sine);
				cosine = _Round_floating_point_to_zero(cosine);
				return{ cosine, sine, sine, -cosine, 0.0F, 0.0F };
			}
			constexpr static matrix_2d init_shear_x(float factor) noexcept {
				return{ 1.0F, 0.0F, factor, 1.0F, 0.0F, 0.0F };
			}
			constexpr static matrix_2d init_shear_y(float factor) noexcept {
				return{ 1.0F, factor, 0.0F, 1.0F, 0.0F, 0.0F };
			}

			// \ref{\iotwod.\matrixtwod.modifiers}, modifiers:
			constexpr matrix_2d& translate(point_2d v) noexcept;
			constexpr matrix_2d& scale(point_2d v) noexcept;
			matrix_2d& rotate(float radians) noexcept;
			matrix_2d& rotate(float radians, point_2d origin) noexcept;
			matrix_2d& reflect(float radians) noexcept;
			constexpr matrix_2d& shear_x(float factor) noexcept;
			constexpr matrix_2d& shear_y(float factor) noexcept;

			// \ref{\iotwod.\matrixtwod.observers}, observers:
			constexpr bool is_finite() const noexcept;
			constexpr bool is_invertible() const noexcept;
			constexpr float determinant() const noexcept;
			constexpr matrix_2d inverse() const noexcept;
			constexpr point_2d transform_pt(point_2d pt) const noexcept;

			// \ref{\iotwod.\matrixtwod.member.ops}, matrix_2d member operators:
			constexpr matrix_2d& operator*=(const matrix_2d& rhs) noexcept;
		};

		// \ref{\iotwod.\matrixtwod.ops}, matrix_2d non-member operators:
		constexpr matrix_2d operator*(const matrix_2d& lhs, const matrix_2d& rhs)
			noexcept;
		constexpr bool operator==(const matrix_2d& lhs, const matrix_2d& rhs)
			noexcept;
		constexpr bool operator!=(const matrix_2d& lhs, const matrix_2d& rhs)
			noexcept;
		constexpr point_2d operator*(point_2d v, const matrix_2d& m)
			noexcept;
		constexpr matrix_2d operator*(const matrix_2d& lhs, const matrix_2d& rhs)
			noexcept;
		constexpr bool operator==(const matrix_2d& lhs, const matrix_2d& rhs)
			noexcept;
		constexpr bool operator!=(const matrix_2d& lhs, const matrix_2d& rhs)
			noexcept;

	}
}