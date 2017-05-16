#pragma once

#ifndef _IO2D_
#define _IO2D_

#define __cpp_lib_experimental_io2d 201707

#include "xio2d.h"

#include <memory>
#include <functional>
#include <exception>
#include <vector>
#include <string>
#include <algorithm>
#include <system_error>
#include <cstdint>
#include <atomic>
#include <variant>
#include <optional>
#include <cmath>
#include <type_traits>
#include <initializer_list>

#ifdef _Filesystem_support_test
#include <filesystem>
#endif

#ifdef _WIN32_WINNT
#include "xinclwindows_h.h"
#elif defined(USE_XCB)
#include <xcb/xcb.h>
#elif defined(USE_XLIB)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

namespace std {
	namespace experimental {
		namespace io2d {
			inline namespace v1 {
				::std::error_code _Cairo_status_t_to_std_error_code(cairo_status_t cs) noexcept;
				void _Throw_if_failed_cairo_status_t(::cairo_status_t);

				constexpr cairo_matrix_t _Cairo_identity_matrix{ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };

				enum class io2d_error {
					success,
					invalid_restore,
					invalid_matrix,
					invalid_status,
					null_pointer,
					invalid_string,
					invalid_path_data,
					read_error,
					write_error,
					surface_finished,
					invalid_dash,
					invalid_index,
					clip_not_representable,
					invalid_stride,
					user_font_immutable,
					user_font_error,
					invalid_clusters,
					device_error,
					invalid_mesh_construction
				};
			}
		}
	}

	template<>
	struct is_error_condition_enum<::std::experimental::io2d::io2d_error>
		: public ::std::true_type { };

	//template<>
	//struct is_error_code_enum<::cairo_status_t>
	//	: public ::std::true_type{ };

	::std::error_condition make_error_condition(experimental::io2d::io2d_error e) noexcept;

	//	::std::error_code make_error_code(cairo_status_t e) noexcept;

	::std::error_code make_error_code(experimental::io2d::io2d_error e) noexcept;

	namespace experimental {
		namespace io2d {
			inline namespace v1 {
				enum class antialias {
					default_antialias,
					none,
					gray,
					subpixel,
					fast,
					good,
					best
				};

				enum class fill_rule {
					winding,
					even_odd
				};

				enum class line_cap {
					none,
					round,
					square
				};

				enum class line_join {
					miter,
					round,
					bevel,
					miter_or_bevel
				};

				enum class compositing_op {
					over,
					clear,
					source,
					in,
					out,
					atop,
					dest,
					dest_over,
					dest_in,
					dest_out,
					dest_atop,
					xor_op,
					add,
					saturate,
					multiply,
					screen,
					overlay,
					darken,
					lighten,
					color_dodge,
					color_burn,
					hard_light,
					soft_light,
					difference,
					exclusion,
					hsl_hue,
					hsl_saturation,
					hsl_color,
					hsl_luminosity
				};

				enum class format {
					invalid,
					argb32,
					xrgb32,
					a8,
					a1,
					rgb16_565,
					rgb30
				};

				enum class wrap_mode {
					none,
					repeat,
					reflect,
					pad
				};

				enum class filter {
					fast,
					good,
					best,
					nearest,
					bilinear
				};

				enum class brush_type {
					solid_color,
					surface,
					linear,
					radial
				};

				enum class scaling {
					letterbox, // Same as uniform except that the display_surface is cleared using the letterbox brush first
					uniform, // Maintain aspect ratio and center as needed, ignoring side areas that are not drawn to
					fill_uniform, // Maintain aspect ratio but fill entire display (some content may not be shown)
					fill_exact, // Ignore aspect ratio and use (possibly non-uniform) scale to fill exactly
					none // Do not scale.
				};

				enum class refresh_rate {
					as_needed,
					as_fast_as_possible,
					fixed
				};

				template <class T>
				constexpr T pi = T(3.14159265358979323846264338327950288L);

				template <class T>
				constexpr T two_pi = T(6.28318530717958647692528676655900576L);

				template <class T>
				constexpr T half_pi = T(1.57079632679489661923132169163975144L);

				template <class T>
				constexpr T three_pi_over_two = T(4.71238898038468985769396507491925432L);

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
#endif

				class io2d_error_category : public ::std::error_category {
				public:
					// Observers
					virtual const char* name() const noexcept override;
					virtual ::std::string message(int errVal) const override;
					virtual bool equivalent(int code, const ::std::error_condition& condition) const noexcept override;
					virtual bool equivalent(const ::std::error_code& ec, int condition) const noexcept override;
				};

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

				const ::std::error_category& io2d_category() noexcept;

				class vector_2d {
					double _X = 0.0;
					double _Y = 0.0;
				public:
					constexpr vector_2d() noexcept { }
					constexpr vector_2d(double x, double y) noexcept
						: _X(x)
						, _Y(y) {
					}
					//constexpr vector_2d(const vector_2d&) noexcept = default;
					//vector_2d(vector_2d&&) noexcept = default;
					//constexpr vector_2d& operator=(const vector_2d&) noexcept = default;
					//vector_2d& operator=(vector_2d&&) noexcept = default;

					constexpr void x(double value) noexcept {
						_X = value;
					}
					constexpr void y(double value) noexcept {
						_Y = value;
					}

					constexpr double x() const noexcept {
						return _X;
					}
					constexpr double y() const noexcept {
						return _Y;
					}

					double magnitude() const noexcept;
					constexpr double magnitude_squared() const noexcept {
						return _X * _X + _Y * _Y;
					}
					constexpr double dot(const vector_2d& other) const noexcept {
						return _X * other._X + _Y * other._Y;
					}
					double angular_direction() const noexcept;
					vector_2d to_unit() const noexcept;

					vector_2d& operator+=(const vector_2d& rhs) noexcept;
					vector_2d& operator-=(const vector_2d& rhs) noexcept;
					vector_2d& operator*=(double rhs) noexcept;
					vector_2d& operator*=(const vector_2d& rhs) noexcept;
					vector_2d& operator/=(double rhs) noexcept;
					vector_2d& operator/=(const vector_2d& rhs) noexcept;
				};

				inline constexpr bool operator==(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return lhs.x() == rhs.x() && lhs.y() == rhs.y();
				}

				inline constexpr bool operator!=(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return !(lhs == rhs);
				}

				inline constexpr vector_2d operator+(const vector_2d& lhs) noexcept {
					return lhs;
				}

				inline constexpr vector_2d operator+(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs.x() + rhs.x(), lhs.y() + rhs.y() };
				}

				inline vector_2d& vector_2d::operator+=(const vector_2d& rhs) noexcept {
					_X = _X + rhs.x();
					_Y = _Y + rhs.y();
					return *this;
				}

				inline constexpr vector_2d operator-(const vector_2d& lhs) noexcept {
					return vector_2d{ -lhs.x(), -lhs.y() };
				}

				inline constexpr vector_2d operator-(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs.x() - rhs.x(), lhs.y() - rhs.y() };
				}

				inline vector_2d& vector_2d::operator-=(const vector_2d& rhs) noexcept {
					_X = _X - rhs.x();
					_Y = _Y - rhs.y();
					return *this;
				}

				inline vector_2d& vector_2d::operator*=(double rhs) noexcept {
					_X *= rhs;
					_Y *= rhs;
					return *this;
				}

				inline vector_2d& vector_2d::operator*=(const vector_2d& rhs) noexcept {
					_X *= rhs.x();
					_Y *= rhs.y();
					return *this;
				}

				inline constexpr vector_2d operator*(const vector_2d& lhs, double rhs) noexcept {
					return vector_2d{ lhs.x() * rhs, lhs.y() * rhs };
				}

				inline constexpr vector_2d operator*(double lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs * rhs.x(), lhs * rhs.y() };
				}

				inline constexpr vector_2d operator*(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs.x() * rhs.x(), lhs.y() * rhs.y() };
				}

				inline vector_2d& vector_2d::operator/=(double rhs) noexcept {
					_X /= rhs;
					_Y /= rhs;
					return *this;
				}

				inline vector_2d& vector_2d::operator/=(const vector_2d& rhs) noexcept {
					_X /= rhs.x();
					_Y /= rhs.y();
					return *this;
				}

				inline constexpr vector_2d operator/(const vector_2d& lhs, double rhs) noexcept {
					return vector_2d{ lhs.x() / rhs, lhs.y() / rhs };
				}

				inline constexpr vector_2d operator/(double lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs / rhs.x(), lhs / rhs.y() };
				}

				inline constexpr vector_2d operator/(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs.x() / rhs.x(), lhs.y() / rhs.y() };
				}

				class rectangle {
					double _X = 0.0;
					double _Y = 0.0;
					double _Width = 0.0;
					double _Height = 0.0;
				public:
					constexpr rectangle() noexcept { }
					constexpr rectangle(double x, double y, double width, double height) noexcept
						: _X(x)
						, _Y(y)
						, _Width(width)
						, _Height(height) {
					}
					constexpr rectangle(const vector_2d& tl, const vector_2d& br) noexcept
						: _X(tl.x())
						, _Y(tl.y())
						, _Width(::std::max(0.0, br.x() - tl.x()))
						, _Height(::std::max(0.0, br.y() - tl.y())) {
					}

					constexpr void x(double value) noexcept {
						_X = value;
					}
					constexpr void y(double value) noexcept {
						_Y = value;
					}
					constexpr void width(double value) noexcept {
						_Width = value;
					}
					constexpr void height(double value) noexcept {
						_Height = value;
					}
					constexpr void top_left(const vector_2d& value) noexcept {
						_X = value.x();
						_Y = value.y();
					}
					constexpr void bottom_right(const vector_2d& value) noexcept {
						_Width = max(0.0, value.x() - _X);
						_Height = max(0.0, value.y() - _Y);
					}

					constexpr double x() const noexcept;
					constexpr double y() const noexcept;
					constexpr double width() const noexcept;
					constexpr double height() const noexcept;
					constexpr double left() const noexcept;
					constexpr double right() const noexcept;
					constexpr double top() const noexcept;
					constexpr double bottom() const noexcept;
					constexpr vector_2d top_left() const noexcept;
					constexpr vector_2d bottom_right() const noexcept;
				};

				inline constexpr double rectangle::x() const noexcept {
					return _X;
				}

				inline constexpr double rectangle::y() const noexcept {
					return _Y;
				}

				inline constexpr double rectangle::width() const noexcept {
					return _Width;
				}

				inline constexpr double rectangle::height() const noexcept {
					return _Height;
				}

				inline constexpr double rectangle::left() const noexcept {
					return _X;
				}

				inline constexpr double rectangle::right() const noexcept {
					return _X + _Width;
				}

				inline constexpr double rectangle::top() const noexcept {
					return _Y;
				}

				inline constexpr double rectangle::bottom() const noexcept {
					return _Y + _Height;
				}

				inline constexpr vector_2d rectangle::top_left() const noexcept {
					return{ _X, _Y };
				}

				inline constexpr vector_2d rectangle::bottom_right() const noexcept {
					return{ _X + _Width, _Y + _Height };
				}

				class ellipse {
					vector_2d _Center;
					vector_2d _Radius;
				public:
					constexpr ellipse() noexcept { }
					constexpr ellipse(const vector_2d& ctr, double r) noexcept
						: _Center(ctr)
						, _Radius({ r, r }) {
					}
					constexpr ellipse(const vector_2d& ctr, const vector_2d& r) noexcept
						: _Center(ctr)
						, _Radius(r) {
					}

					constexpr void center(const vector_2d& ctr) noexcept {
						_Center = ctr;
					}
					constexpr void radius(const vector_2d& r) noexcept {
						_Radius = r;
					}
					constexpr void x_radius(double val) noexcept {
						_Radius.x(val);
					}
					constexpr void y_radius(double val) noexcept {
						_Radius.y(val);
					}

					constexpr vector_2d center() const noexcept {
						return _Center;
					}
					constexpr vector_2d radius() const noexcept {
						return _Radius;
					}
					constexpr double x_radius() const noexcept {
						return _Radius.x();
					}
					constexpr double y_radius() const noexcept {
						return _Radius.y();
					}
				};

				class circle {
					ellipse _Ellipse;
				public:
					constexpr circle() noexcept {}
					constexpr circle(const vector_2d& ctr, double r) noexcept
						: _Ellipse(ctr, { r, r }) {}

					constexpr void center(const vector_2d& ctr) noexcept {
						_Ellipse.center(ctr);
					}
					constexpr void radius(double r) noexcept {
						_Ellipse.radius({ r, r });
					}

					constexpr vector_2d center() const noexcept {
						return _Ellipse.center();
					}
					constexpr double radius() const noexcept {
						return _Ellipse.radius().x();
					}
				};
				//enum class _Color_is_integral {};
				//constexpr _Color_is_integral _Color_is_integral_val{};
				//enum class _Color_is_floating {};
				//constexpr _Color_is_floating _Color_is_floating_val{};

				class bgra_color {
					double _R = 0.0;
					double _G = 0.0;
					double _B = 0.0;
					double _A = 1.0;
				public:
					constexpr bgra_color() noexcept { }
					constexpr bgra_color(double r, double g, double b, double a = 1.0) noexcept
						: _R(r)
						, _G(g)
						, _B(b)
						, _A(a) { }
					//template <class T, ::std::enable_if_t<::std::is_integral_v<T>, _Color_is_integral> = _Color_is_integral_val>
					//constexpr bgra_color(T r, T g, T b, T a = 0xFF)
					//	: _R(static_cast<double>(::std::min<double>(::std::max<double>((r / 255.0), 0.0), 1.0)))
					//	, _G(static_cast<double>(::std::min<double>(::std::max<double>((g / 255.0), 0.0), 1.0)))
					//	, _B(static_cast<double>(::std::min<double>(::std::max<double>((b / 255.0), 0.0), 1.0)))
					//	, _A(static_cast<double>(::std::min<double>(::std::max<double>((a / 255.0), 0.0), 1.0))) { }
					//template <class T, ::std::enable_if_t<::std::is_floating_point_v<T>, _Color_is_floating> = _Color_is_floating_val>
					//constexpr bgra_color(T r, T g, T b, T a = 1.0)
					//	: _R(static_cast<double>(::std::min<T>(::std::max<T>(static_cast<double>(r), 0.0), 1.0)))
					//	, _G(static_cast<double>(::std::min<T>(::std::max<T>(static_cast<double>(g), 0.0), 1.0)))
					//	, _B(static_cast<double>(::std::min<T>(::std::max<T>(static_cast<double>(b), 0.0), 1.0)))
					//	, _A(static_cast<double>(::std::min<T>(::std::max<T>(static_cast<double>(a), 0.0), 1.0))) {
					//}

					void r(double val) noexcept {
						_R = val * _A;
					}
					void g(double val) noexcept {
						_G = val * _A;
					}
					void b(double val) noexcept {
						_B = val * _A;
					}
					void a(double val) noexcept {
						_A = val;
					}

					constexpr double r() const noexcept {
						return _R;
					}
					constexpr double g() const noexcept {
						return _G;
					}
					constexpr double b() const noexcept {
						return _B;
					}
					constexpr double a() const noexcept {
						return _A;
					}

					static const bgra_color& alice_blue() noexcept;
					static const bgra_color& antique_white() noexcept;
					static const bgra_color& aqua() noexcept;
					static const bgra_color& aquamarine() noexcept;
					static const bgra_color& azure() noexcept;
					static const bgra_color& beige() noexcept;
					static const bgra_color& bisque() noexcept;
					static const bgra_color& black() noexcept;
					static const bgra_color& blanched_almond() noexcept;
					static const bgra_color& blue() noexcept;
					static const bgra_color& blue_violet() noexcept;
					static const bgra_color& brown() noexcept;
					static const bgra_color& burly_wood() noexcept;
					static const bgra_color& cadet_blue() noexcept;
					static const bgra_color& chartreuse() noexcept;
					static const bgra_color& chocolate() noexcept;
					static const bgra_color& coral() noexcept;
					static const bgra_color& cornflower_blue() noexcept;
					static const bgra_color& cornsilk() noexcept;
					static const bgra_color& crimson() noexcept;
					static const bgra_color& cyan() noexcept;
					static const bgra_color& dark_blue() noexcept;
					static const bgra_color& dark_cyan() noexcept;
					static const bgra_color& dark_goldenrod() noexcept;
					static const bgra_color& dark_gray() noexcept;
					static const bgra_color& dark_green() noexcept;
					static const bgra_color& dark_grey() noexcept;
					static const bgra_color& dark_khaki() noexcept;
					static const bgra_color& dark_magenta() noexcept;
					static const bgra_color& dark_olive_green() noexcept;
					static const bgra_color& dark_orange() noexcept;
					static const bgra_color& dark_orchid() noexcept;
					static const bgra_color& dark_red() noexcept;
					static const bgra_color& dark_salmon() noexcept;
					static const bgra_color& dark_sea_green() noexcept;
					static const bgra_color& dark_slate_blue() noexcept;
					static const bgra_color& dark_slate_gray() noexcept;
					static const bgra_color& dark_slate_grey() noexcept;
					static const bgra_color& dark_turquoise() noexcept;
					static const bgra_color& dark_violet() noexcept;
					static const bgra_color& deep_pink() noexcept;
					static const bgra_color& deep_sky_blue() noexcept;
					static const bgra_color& dim_gray() noexcept;
					static const bgra_color& dim_grey() noexcept;
					static const bgra_color& dodger_blue() noexcept;
					static const bgra_color& firebrick() noexcept;
					static const bgra_color& floral_white() noexcept;
					static const bgra_color& forest_green() noexcept;
					static const bgra_color& fuchsia() noexcept;
					static const bgra_color& gainsboro() noexcept;
					static const bgra_color& ghost_white() noexcept;
					static const bgra_color& gold() noexcept;
					static const bgra_color& goldenrod() noexcept;
					static const bgra_color& gray() noexcept;
					static const bgra_color& green() noexcept;
					static const bgra_color& green_yellow() noexcept;
					static const bgra_color& grey() noexcept;
					static const bgra_color& honeydew() noexcept;
					static const bgra_color& hot_pink() noexcept;
					static const bgra_color& indian_red() noexcept;
					static const bgra_color& indigo() noexcept;
					static const bgra_color& ivory() noexcept;
					static const bgra_color& khaki() noexcept;
					static const bgra_color& lavender() noexcept;
					static const bgra_color& lavender_blush() noexcept;
					static const bgra_color& lawn_green() noexcept;
					static const bgra_color& lemon_chiffon() noexcept;
					static const bgra_color& light_blue() noexcept;
					static const bgra_color& light_coral() noexcept;
					static const bgra_color& light_cyan() noexcept;
					static const bgra_color& light_goldenrod_yellow() noexcept;
					static const bgra_color& light_gray() noexcept;
					static const bgra_color& light_green() noexcept;
					static const bgra_color& light_grey() noexcept;
					static const bgra_color& light_pink() noexcept;
					static const bgra_color& light_salmon() noexcept;
					static const bgra_color& light_sea_green() noexcept;
					static const bgra_color& light_sky_blue() noexcept;
					static const bgra_color& light_slate_gray() noexcept;
					static const bgra_color& light_slate_grey() noexcept;
					static const bgra_color& light_steel_blue() noexcept;
					static const bgra_color& light_yellow() noexcept;
					static const bgra_color& lime() noexcept;
					static const bgra_color& lime_green() noexcept;
					static const bgra_color& linen() noexcept;
					static const bgra_color& magenta() noexcept;
					static const bgra_color& maroon() noexcept;
					static const bgra_color& medium_aquamarine() noexcept;
					static const bgra_color& medium_blue() noexcept;
					static const bgra_color& medium_orchid() noexcept;
					static const bgra_color& medium_purple() noexcept;
					static const bgra_color& medium_sea_green() noexcept;
					static const bgra_color& medium_slate_blue() noexcept;
					static const bgra_color& medium_spring_green() noexcept;
					static const bgra_color& medium_turquoise() noexcept;
					static const bgra_color& medium_violet_red() noexcept;
					static const bgra_color& midnight_blue() noexcept;
					static const bgra_color& mint_cream() noexcept;
					static const bgra_color& misty_rose() noexcept;
					static const bgra_color& moccasin() noexcept;
					static const bgra_color& navajo_white() noexcept;
					static const bgra_color& navy() noexcept;
					static const bgra_color& old_lace() noexcept;
					static const bgra_color& olive() noexcept;
					static const bgra_color& olive_drab() noexcept;
					static const bgra_color& orange() noexcept;
					static const bgra_color& orange_red() noexcept;
					static const bgra_color& orchid() noexcept;
					static const bgra_color& pale_goldenrod() noexcept;
					static const bgra_color& pale_green() noexcept;
					static const bgra_color& pale_turquoise() noexcept;
					static const bgra_color& pale_violet_red() noexcept;
					static const bgra_color& papaya_whip() noexcept;
					static const bgra_color& peach_puff() noexcept;
					static const bgra_color& peru() noexcept;
					static const bgra_color& pink() noexcept;
					static const bgra_color& plum() noexcept;
					static const bgra_color& powder_blue() noexcept;
					static const bgra_color& purple() noexcept;
					static const bgra_color& red() noexcept;
					static const bgra_color& rosy_brown() noexcept;
					static const bgra_color& royal_blue() noexcept;
					static const bgra_color& saddle_brown() noexcept;
					static const bgra_color& salmon() noexcept;
					static const bgra_color& sandy_brown() noexcept;
					static const bgra_color& sea_green() noexcept;
					static const bgra_color& sea_shell() noexcept;
					static const bgra_color& sienna() noexcept;
					static const bgra_color& silver() noexcept;
					static const bgra_color& sky_blue() noexcept;
					static const bgra_color& slate_blue() noexcept;
					static const bgra_color& slate_gray() noexcept;
					static const bgra_color& slate_grey() noexcept;
					static const bgra_color& snow() noexcept;
					static const bgra_color& spring_green() noexcept;
					static const bgra_color& steel_blue() noexcept;
					static const bgra_color& tan() noexcept;
					static const bgra_color& teal() noexcept;
					static const bgra_color& thistle() noexcept;
					static const bgra_color& tomato() noexcept;
					static const bgra_color& transparent_black() noexcept; // Note: Not in CSS3.
					static const bgra_color& turquoise() noexcept;
					static const bgra_color& violet() noexcept;
					static const bgra_color& wheat() noexcept;
					static const bgra_color& white() noexcept;
					static const bgra_color& white_smoke() noexcept;
					static const bgra_color& yellow() noexcept;
					static const bgra_color& yellow_green() noexcept;
				};

				inline constexpr bool operator==(const bgra_color& lhs, const bgra_color& rhs) {
					return lhs.r() == rhs.r() && lhs.g() == rhs.g() && lhs.b() == rhs.b() && lhs.a() == rhs.a();
				}
				inline constexpr bool operator!=(const bgra_color& lhs, const bgra_color& rhs) {
					return !(lhs == rhs);
				}

				inline constexpr bgra_color operator*(const bgra_color& lhs, double rhs) {
					rhs = ::std::max(::std::min(rhs, 1.0), 0.0);
					return{
						::std::min(lhs.r() * rhs, 1.0),
						::std::min(lhs.g() * rhs, 1.0),
						::std::min(lhs.b() * rhs, 1.0),
						::std::min(lhs.a() * rhs, 1.0)
					};
				}

				inline constexpr bgra_color operator*(double lhs, const bgra_color& rhs) {
					lhs = ::std::max(::std::min(lhs, 1.0), 0.0);
					return{
						::std::min(lhs * rhs.r(), 1.0),
						::std::min(lhs * rhs.g(), 1.0),
						::std::min(lhs * rhs.b(), 1.0),
						::std::min(lhs * rhs.a(), 1.0)
					};
				}


				inline namespace literals {
					// Note: The _ prefix is added because certain compilers reject attempts to add a non-user-defined literal
					inline double operator""_ubyte(unsigned long long value) noexcept {
						return ::std::max(0.0, ::std::min(1.0, static_cast<double>(value) / 255.0));
					}

					// Note: The _ prefix is added because certain compilers reject attempts to add a non-user-defined literal
					inline double operator "" _unorm(long double value) noexcept {
						auto result = ::std::max(0.0, ::std::min(1.0, static_cast<double>(value)));
						result = ::std::nearbyint(result * 255.0); // We need to ensure it is one of the discrete values between 0 and 255.
						return result / 255.0;
					}
				}

				class matrix_2d {
					double _M00 = 1.0;
					double _M01 = 0.0;
					double _M10 = 0.0;
					double _M11 = 1.0;
					double _M20 = 0.0;
					double _M21 = 0.0;
				public:

					constexpr matrix_2d() noexcept = default;
					constexpr matrix_2d(double m00, double m01, double m10, double m11, double m20, double m21) noexcept {
						_M00 = m00;
						_M01 = m01;
						_M10 = m10;
						_M11 = m11;
						_M20 = m20;
						_M21 = m21;
					}

					constexpr static matrix_2d init_identity() noexcept {
						return{ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
					}
					constexpr static matrix_2d init_translate(const vector_2d& value) noexcept {
						return{ 1.0, 0.0, 0.0, 1.0, value.x(), value.y() };
					}
					constexpr static matrix_2d init_scale(const vector_2d& value) noexcept {
						return{ value.x(), 0.0, 0.0, value.y(), 0.0, 0.0 };
					}
					static matrix_2d init_rotate(double radians) noexcept;
					constexpr static matrix_2d init_shear_x(double factor) noexcept {
						return{ 1.0, 0.0, factor, 1.0, 0.0, 0.0 };
					}
					constexpr static matrix_2d init_shear_y(double factor) noexcept {
						return{ 1.0, factor, 0.0, 1.0, 0.0, 0.0 };
					}

					// Modifiers
					constexpr void m00(double value) noexcept {
						_M00 = value;
					}
					constexpr void m01(double value) noexcept {
						_M01 = value;
					}
					constexpr void m10(double value) noexcept {
						_M10 = value;
					}
					constexpr void m11(double value) noexcept {
						_M11 = value;
					}
					constexpr void m20(double value) noexcept {
						_M20 = value;
					}
					constexpr void m21(double value) noexcept {
						_M21 = value;
					}
					matrix_2d& translate(const vector_2d& value) noexcept;
					matrix_2d& scale(const vector_2d& value) noexcept;
					matrix_2d& rotate(double radians) noexcept;
					matrix_2d& shear_x(double factor) noexcept;
					matrix_2d& shear_y(double factor) noexcept;
					matrix_2d& invert();
					matrix_2d& invert(::std::error_code& ec) noexcept;

					// Observers
					constexpr double m00() const noexcept {
						return _M00;
					}
					constexpr double m01() const noexcept {
						return _M01;
					}
					constexpr double m10() const noexcept {
						return _M10;
					}
					constexpr double m11() const noexcept {
						return _M11;
					}
					constexpr double m20() const noexcept {
						return _M20;
					}
					constexpr double m21() const noexcept {
						return _M21;
					}
					constexpr bool is_invertible() const noexcept {
						return (_M00 * _M11 - _M01 * _M10) != 0.0;
					}
				private:
					constexpr bool _Is_finite_check(double val) const noexcept {
						return val != numeric_limits<double>::infinity() &&
							val != -numeric_limits<double>::infinity() &&
							!(val != val); // This checks for both types of NaN. Compilers should not optimize this away but the only way to be sure is to check the documentation and any compiler switches you may be using.
					}
				public:
					constexpr bool is_finite() const noexcept {
						static_assert(numeric_limits<double>::is_iec559 == true, "This implementation relies on IEEE 754 floating point behavior.");
						return numeric_limits<double>::is_iec559 &&
							_Is_finite_check(_M00) &&
							_Is_finite_check(_M01) &&
							_Is_finite_check(_M10) &&
							_Is_finite_check(_M11);
					}
					constexpr double determinant() const noexcept {
						return _M00 * _M11 - _M01 * _M10;
					}
					constexpr vector_2d transform_point(const vector_2d& pt) const noexcept {
						auto result = vector_2d{ _M00 * pt.x() + _M10 * pt.y() + _M20, _M01 * pt.x() + _M11 * pt.y() + _M21 };
						if ((result.x() < numeric_limits<double>::epsilon() * 100.0) &&
							(-result.x() < numeric_limits<double>::epsilon() * 100.0)) {
							result.x(result.x() < 0 ? -0.0 : 0.0);
						}
						if ((result.y() < numeric_limits<double>::epsilon() * 100.0) &&
							(-result.y() < numeric_limits<double>::epsilon() * 100.0)) {
							result.y(result.y() < 0 ? -0.0 : 0.0);
						}
						return result;
					}

					matrix_2d& operator*=(const matrix_2d& rhs) noexcept;
				};

				constexpr matrix_2d operator*(const matrix_2d& lhs, const matrix_2d& rhs) noexcept {
					return matrix_2d{
						(lhs.m00() * rhs.m00()) + (lhs.m01() * rhs.m10()),
						(lhs.m00() * rhs.m01()) + (lhs.m01() * rhs.m11()),
						(lhs.m10() * rhs.m00()) + (lhs.m11() * rhs.m10()),
						(lhs.m10() * rhs.m01()) + (lhs.m11() * rhs.m11()),
						(lhs.m20() * rhs.m00()) + (lhs.m21() * rhs.m10()) + rhs.m20(),
						(lhs.m20() * rhs.m01()) + (lhs.m21() * rhs.m11()) + rhs.m21()
					};
				}
				constexpr bool operator==(const matrix_2d& lhs, const matrix_2d& rhs) noexcept {
					return lhs.m00() == rhs.m00() && lhs.m01() == rhs.m01() &&
						lhs.m10() == rhs.m10() && lhs.m11() == rhs.m11() &&
						lhs.m20() == rhs.m20() && lhs.m21() == rhs.m21();
				}
				constexpr bool operator!=(const matrix_2d& lhs, const matrix_2d& rhs) noexcept {
					return !(lhs == rhs);
				}


				namespace path_data {
					class abs_new_path {
						vector_2d _Data = {};
					public:
						constexpr abs_new_path() noexcept {}

						constexpr abs_new_path(const vector_2d& pt) noexcept
							: _Data(pt) {}

						constexpr void at(const vector_2d& pt) noexcept {
							_Data = pt;
						}

						constexpr vector_2d at() const noexcept {
							return _Data;
						}
					};

					class rel_new_path {
						vector_2d _Data = {};
					public:
						constexpr rel_new_path() noexcept {}

						constexpr rel_new_path(const vector_2d& pt) noexcept
							: _Data(pt) {}

						constexpr void at(const vector_2d& pt) noexcept {
							_Data = pt;
						}

						constexpr vector_2d at() const noexcept {
							return _Data;
						}
					};

					class close_path {
						vector_2d _Data = {};
					public:
						constexpr explicit close_path(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr close_path() noexcept {}

						constexpr void to(const vector_2d& value) noexcept {
							_Data = value;
						}

						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class abs_line {
						vector_2d _Data = {};
					public:
						constexpr explicit abs_line(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr abs_line() noexcept {}

						constexpr void to(const vector_2d& value) noexcept {
							_Data = value;
						}

						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class rel_line {
						vector_2d _Data = {};
					public:
						constexpr explicit rel_line(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr rel_line() noexcept {}

						constexpr void to(const vector_2d& value) noexcept {
							_Data = value;
						}

						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class abs_cubic_curve {
						vector_2d _Control_pt1 = {};
						vector_2d _Control_pt2 = {};
						vector_2d _End_pt = {};
					public:
						constexpr abs_cubic_curve(const vector_2d& controlPoint1, const vector_2d& controlPoint2, const vector_2d& endPoint) noexcept
							: _Control_pt1(controlPoint1)
							, _Control_pt2(controlPoint2)
							, _End_pt(endPoint) {
						}
						constexpr abs_cubic_curve() noexcept {}

						constexpr void control_point_1(const vector_2d& value) noexcept {
							_Control_pt1 = value;
						}
						constexpr void control_point_2(const vector_2d& value) noexcept {
							_Control_pt2 = value;
						}
						constexpr void end_point(const vector_2d& value) noexcept {
							_End_pt = value;
						}

						constexpr vector_2d control_point_1() const noexcept {
							return _Control_pt1;
						}
						constexpr vector_2d control_point_2() const noexcept {
							return _Control_pt2;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class rel_cubic_curve {
						vector_2d _Control_pt1 = {};
						vector_2d _Control_pt2 = {};
						vector_2d _End_pt = {};
					public:
						constexpr rel_cubic_curve(const vector_2d& controlPoint1, const vector_2d& controlPoint2, const vector_2d& endPoint) noexcept
							: _Control_pt1(controlPoint1)
							, _Control_pt2(controlPoint2)
							, _End_pt(endPoint) {
						}
						constexpr rel_cubic_curve() noexcept {}

						constexpr void control_point_1(const vector_2d& value) noexcept {
							_Control_pt1 = value;
						}
						constexpr void control_point_2(const vector_2d& value) noexcept {
							_Control_pt2 = value;
						}
						constexpr void end_point(const vector_2d& value) noexcept {
							_End_pt = value;
						}

						constexpr vector_2d control_point_1() const noexcept {
							return _Control_pt1;
						}
						constexpr vector_2d control_point_2() const noexcept {
							return _Control_pt2;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class abs_quadratic_curve {
						vector_2d _Control_pt = {};
						vector_2d _End_pt = {};
					public:
						constexpr abs_quadratic_curve(const vector_2d& cp, const vector_2d& ep) noexcept
							: _Control_pt(cp)
							, _End_pt(ep) {
						}
						constexpr abs_quadratic_curve() noexcept {}

						constexpr void control_point(const vector_2d& value) noexcept {
							_Control_pt = value;
						}
						constexpr void end_point(const vector_2d& value) noexcept {
							_End_pt = value;
						}

						constexpr vector_2d control_point() const noexcept {
							return _Control_pt;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class rel_quadratic_curve {
						vector_2d _Control_pt = {};
						vector_2d _End_pt = {};
					public:
						constexpr rel_quadratic_curve(const vector_2d& cp, const vector_2d& ep) noexcept
							: _Control_pt(cp)
							, _End_pt(ep) {
						}
						constexpr rel_quadratic_curve() noexcept {}

						constexpr void control_point(const vector_2d& value) noexcept {
							_Control_pt = value;
						}
						constexpr void end_point(const vector_2d& value) noexcept {
							_End_pt = value;
						}

						constexpr vector_2d control_point() const noexcept {
							return _Control_pt;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class arc {
						vector_2d _Radius;
						double _Rotation;
						double _Start_angle;
					public:
						constexpr arc() noexcept
							: arc(vector_2d{ 10.0, 10.0 }, pi<double>, pi<double>) { }
						constexpr arc(const vector_2d& rad, double rot, double sang = pi<double>) noexcept
							: _Radius(rad)
							, _Rotation(rot)
							, _Start_angle(sang) {
						}

						constexpr void radius(const vector_2d& rad) noexcept {
							_Radius = rad;
						}
						constexpr void rotation(double rot) noexcept {
							_Rotation = rot;
						}
						constexpr void start_angle(double sang) noexcept {
							_Start_angle = sang;
						}

						constexpr vector_2d radius() const noexcept {
							return _Radius;
						}
						constexpr double rotation() const noexcept {
							return _Rotation;
						}
						constexpr double start_angle() const noexcept {
							return _Start_angle;
						}
					};

					class change_matrix {
						matrix_2d _Matrix;
					public:
						constexpr explicit change_matrix(const matrix_2d& m) noexcept
							: _Matrix(m) {
						}
						constexpr change_matrix() noexcept {}

						constexpr void matrix(const matrix_2d& value) noexcept {
							_Matrix = value;
						}
						constexpr matrix_2d matrix() const noexcept {
							return _Matrix;
						}
					};

					class change_origin {
						vector_2d _Origin = {};
					public:
						constexpr explicit change_origin(const vector_2d& origin) noexcept
							: _Origin(origin) {
						}
						constexpr change_origin() noexcept {}

						constexpr void origin(const vector_2d& value) noexcept {
							_Origin = value;
						}
						constexpr vector_2d origin() const noexcept {
							return _Origin;
						}
					};

					using path_data_types = ::std::variant<abs_new_path, rel_new_path, close_path, change_matrix, change_origin, abs_cubic_curve, abs_line, abs_quadratic_curve, arc, rel_cubic_curve, rel_cubic_curve, rel_line, rel_quadratic_curve>;
				}

				// Forward declaration.
				class surface;
				template <class Allocator = allocator<path_data::path_data_types>>
				class path_builder;

				class path_group {
					::std::shared_ptr<cairo_path_t> _Cairo_path;
				public:
					using native_handle_type = cairo_path*;
					// Note: Can default construct. It will just be empty. To be useful it would need to be assigned to.
					path_group() noexcept = default;
					template <class Allocator>
					explicit path_group(const path_builder<Allocator>& p);
					template <class Allocator>
					path_group(const path_builder<Allocator>& p, std::error_code& ec) noexcept;
					//path_group(const path_group&) = default;
					//path_group& operator=(const path_group&) = default;
					//path_group(path_group&&) = default;
					//path_group& operator=(path_group&&) = default;

					native_handle_type native_handle() {
						return _Cairo_path.get();
					}
					const cairo_path* native_handle() const {
						return _Cairo_path.get();
					}
				};

				enum class _Path_data_abs_new_path {};
				constexpr static _Path_data_abs_new_path _Path_data_abs_new_path_val = {};
				enum class _Path_data_rel_new_path {};
				constexpr static _Path_data_rel_new_path _Path_data_rel_new_path_val = {};
				enum class _Path_data_close_path {};
				constexpr static _Path_data_close_path _Path_data_close_path_val = {};
				enum class _Path_data_change_matrix {};
				constexpr static _Path_data_change_matrix _Path_data_change_matrix_val = {};
				enum class _Path_data_change_origin {};
				constexpr static _Path_data_change_origin _Path_data_change_origin_val = {};
				enum class _Path_data_abs_cubic_curve {};
				constexpr static _Path_data_abs_cubic_curve _Path_data_abs_cubic_curve_val = {};
				enum class _Path_data_abs_line {};
				constexpr static _Path_data_abs_line _Path_data_abs_line_val = {};
				enum class _Path_data_abs_quadratic_curve {};
				constexpr static _Path_data_abs_quadratic_curve _Path_data_abs_quadratic_curve_val = {};
				enum class _Path_data_arc {};
				constexpr static _Path_data_arc _Path_data_arc_val = {};
				enum class _Path_data_rel_cubic_curve {};
				constexpr static _Path_data_rel_cubic_curve _Path_data_rel_cubic_curve_val = {};
				enum class _Path_data_rel_line {};
				constexpr static _Path_data_rel_line _Path_data_rel_line_val = {};
				enum class _Path_data_rel_quadratic_curve {};
				constexpr static _Path_data_rel_quadratic_curve _Path_data_rel_quadratic_curve_val = {};

				template <class _TItem>
				struct _Path_group_perform_visit {
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_new_path>, _Path_data_abs_new_path> = _Path_data_abs_new_path_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::abs_new_path& item) noexcept {
						cairo_path_data_t cpdItem{};
						auto pt = item.at();
						cpdItem.header.type = CAIRO_PATH_MOVE_TO;
						cpdItem.header.length = 2;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt.x(), pt.y() };
						vec.push_back(cpdItem);
					}

					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_line>, _Path_data_abs_line> = _Path_data_abs_line_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::abs_line& item) noexcept {
						cairo_path_data_t cpdItem{};
						auto pt = item.to();
						cpdItem.header.type = CAIRO_PATH_LINE_TO;
						cpdItem.header.length = 2;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt.x(), pt.y() };
						vec.push_back(cpdItem);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_cubic_curve>, _Path_data_abs_cubic_curve> = _Path_data_abs_cubic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::abs_cubic_curve& item) noexcept {
						cairo_path_data_t cpdItem{};
						auto pt1 = item.control_point_1();
						auto pt2 = item.control_point_2();
						auto pt3 = item.end_point();
						cpdItem.header.type = CAIRO_PATH_CURVE_TO;
						cpdItem.header.length = 4;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt1.x(), pt1.y() };
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt2.x(), pt2.y() };
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt3.x(), pt3.y() };
						vec.push_back(cpdItem);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_quadratic_curve>, _Path_data_abs_quadratic_curve> = _Path_data_abs_quadratic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::abs_quadratic_curve&) noexcept {
						assert(false && "Abs quadratic curves should have been transformed into cubic curves already.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_new_path>, _Path_data_rel_new_path> = _Path_data_rel_new_path_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_new_path&) noexcept {
						assert(false && "Rel new path instructions should have been eliminated.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::close_path>, _Path_data_close_path> = _Path_data_close_path_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::close_path& item) noexcept {
						auto lastMoveToPoint = item.to();
						cairo_path_data_t cpdItem{};
						cpdItem.header.type = CAIRO_PATH_CLOSE_PATH;
						cpdItem.header.length = 1;
						vec.push_back(cpdItem);
						cpdItem.header.type = CAIRO_PATH_MOVE_TO;
						cpdItem.header.length = 2;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { lastMoveToPoint.x(), lastMoveToPoint.y() };
						vec.push_back(cpdItem);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_line>, _Path_data_rel_line> = _Path_data_rel_line_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_line&) noexcept {
						assert(false && "Rel line should have been transformed into non-relative.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_cubic_curve>, _Path_data_rel_cubic_curve> = _Path_data_rel_cubic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_cubic_curve&) noexcept {
						assert(false && "Rel curve should have been transformed into non-relative.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_quadratic_curve>, _Path_data_rel_quadratic_curve> = _Path_data_rel_quadratic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_quadratic_curve&) noexcept {
						assert(false && "Rel quadratic curves should have been transformed into cubic curves.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc>, _Path_data_arc> = _Path_data_arc_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::arc&) noexcept {
						assert(false && "Arcs should have been transformed into cubic curves.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_matrix>, _Path_data_change_matrix> = _Path_data_change_matrix_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::change_matrix&) noexcept {
						assert(false && "Change matrix should have been eliminated.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_origin>, _Path_data_change_origin> = _Path_data_change_origin_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::change_origin&) noexcept {
						assert(false && "Change origin should have been eliminated.");
					}
				};

				template <class Allocator>
				::std::vector<path_data::path_data_types> _Process_path_data(const path_builder<Allocator>&);
				template <class Allocator>
				::std::vector<path_data::path_data_types> _Process_path_data(const path_builder<Allocator>&, ::std::error_code&) noexcept;

				template <class Allocator>
				inline path_group::path_group(const path_builder<Allocator>& pf)
					: _Cairo_path(new cairo_path_t, [](cairo_path_t* path) {
					if (path != nullptr) {
						if (path->data != nullptr) {
							delete[] path->data;
							path->data = nullptr;
							path->status = CAIRO_STATUS_NULL_POINTER;
						}
						delete path;
						path = nullptr;
					}
				}) {
					auto processedVec = _Process_path_data<Allocator>(pf);
					::std::vector<cairo_path_data_t> vec;

					for (const auto& val : processedVec) {
						::std::visit([&vec](auto&& item) {
							using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
							_Path_group_perform_visit<T>::template _Perform<T>(vec, item);
						}, val);
					}
					_Cairo_path->num_data = static_cast<int>(vec.size());
					const auto numDataST = vec.size();
					_Cairo_path->data = new cairo_path_data_t[numDataST];
					for (size_t currItemIndex = 0; currItemIndex < numDataST; currItemIndex++) {
						_Cairo_path->data[currItemIndex] = vec[currItemIndex];
					}
					_Cairo_path->status = CAIRO_STATUS_SUCCESS;
				}

				template<class Allocator>
				inline path_group::path_group(const path_builder<Allocator>& pf, ::std::error_code& ec) noexcept
					: _Cairo_path(new cairo_path_t, [](cairo_path_t* path) {
					if (path != nullptr) {
						if (path->data != nullptr) {
							delete[] path->data;
							path->data = nullptr;
							path->status = CAIRO_STATUS_NULL_POINTER;
						}
						delete path;
						path = nullptr;
					}
				}) {
					auto processedVec = _Process_path_data<Allocator>(pf, ec);
					if (static_cast<bool>(ec)) {
						return;
					}

					::std::vector<cairo_path_data_t> vec;

					for (const auto& val : processedVec) {
						::std::visit([&vec](auto&& item) {
							using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
							_Path_group_perform_visit<T>::template _Perform<T>(vec, item);
						}, val);
					}
					_Cairo_path->num_data = static_cast<int>(vec.size());
					const auto numDataST = vec.size();
					_Cairo_path->data = new cairo_path_data_t[numDataST];
					for (size_t currItemIndex = 0; currItemIndex < numDataST; currItemIndex++) {
						_Cairo_path->data[currItemIndex] = vec[currItemIndex];
					}
					_Cairo_path->status = CAIRO_STATUS_SUCCESS;
					ec.clear();
				}

				template <class Allocator>
				class path_builder {
					::std::vector<path_data::path_data_types, Allocator> _Data;
					optional<vector_2d> _Current_point;
					vector_2d _Last_move_to_point;
					matrix_2d _Transform_matrix;
					vector_2d _Origin;
				public:
					using value_type = path_data::path_data_types;
					using allocator_type = Allocator;
					using reference = value_type&;
					using const_reference = const value_type&;
					using size_type = ::std::vector<path_data::path_data_types>::size_type;
					using difference_type = ::std::vector<path_data::path_data_types>::difference_type;
					using iterator = ::std::vector<path_data::path_data_types>::iterator;
					using const_iterator = ::std::vector<path_data::path_data_types>::const_iterator;
					using reverse_iterator = std::reverse_iterator<iterator>;
					using const_reverse_iterator = std::reverse_iterator<const_iterator>;

					path_builder() noexcept(noexcept(Allocator())) :
						path_builder(Allocator()) { }
					explicit path_builder(const Allocator& a) noexcept;
					explicit path_builder(size_type n, const Allocator& a = Allocator());
					path_builder(size_type n, const value_type& value,
						const Allocator& a = Allocator());
					template <class InputIterator>
					path_builder(InputIterator first, InputIterator last,
						const Allocator& = Allocator());
					path_builder(const path_builder& pf);
					path_builder(path_builder&& pf) noexcept;
					path_builder(const path_builder& pf, const Allocator& a);
					path_builder(path_builder&& pf, const Allocator& a);
					path_builder(initializer_list<value_type> il, const Allocator& a = Allocator());
					~path_builder() { }
					path_builder& operator=(const path_builder& x);
					path_builder& operator=(path_builder&& x)
						noexcept(
							allocator_traits<Allocator>::propagate_on_container_move_assignment::value
							||
							allocator_traits<Allocator>::is_always_equal::value);
					path_builder& operator=(initializer_list<value_type> il);
					template <class InputIterator>
					void assign(InputIterator first, InputIterator last);
					void assign(size_type n, const value_type& u);
					void assign(initializer_list<value_type> il);
					allocator_type get_allocator() const noexcept;

					iterator begin() noexcept;
					const_iterator begin() const noexcept;
					const_iterator cbegin() const noexcept;
					iterator end() noexcept;
					const_iterator end() const noexcept;
					const_iterator cend() const noexcept;
					reverse_iterator rbegin() noexcept;
					const_reverse_iterator rbegin() const noexcept;
					const_reverse_iterator crbegin() const noexcept;
					reverse_iterator rend() noexcept;
					const_reverse_iterator rend() const noexcept;
					const_reverse_iterator crend() const noexcept;

					bool empty() const noexcept;
					size_type size() const noexcept;
					size_type max_size() const noexcept;
					size_type capacity() const noexcept;
					void resize(size_type sz);
					void resize(size_type sz, const value_type& c);
					void reserve(size_type n);
					void shrink_to_fit();

					reference operator[](size_type n);
					const_reference operator[](size_type n) const;
					const_reference at(size_type n) const;
					reference at(size_type n);
					reference front();
					const_reference front() const;
					reference back();
					const_reference back() const;

					// Modifiers
					void new_path(const vector_2d& v) noexcept;
					void rel_new_path(const vector_2d& v) noexcept;
					void close_path() noexcept;
					void arc(const vector_2d& radius, double rot, double sang = pi<double>) noexcept;
					void cubic_curve_to(const vector_2d& pt0, const vector_2d& pt1,
						const vector_2d& pt2) noexcept;
					void line_to(const vector_2d& pt) noexcept;
					void quadratic_curve_to(const vector_2d& pt0, const vector_2d& pt1)
						noexcept;
					void rel_cubic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1,
						const vector_2d& dpt2) noexcept;
					void rel_line_to(const vector_2d& dpt) noexcept;
					void rel_quadratic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1)
						noexcept;
					void transform_matrix(const matrix_2d& m) noexcept;
					void origin(const vector_2d& pt) noexcept;
					template <class... Args>
					reference emplace_back(Args&&... args);
					void push_back(const value_type& x);
					void push_back(value_type&& x);
					void pop_back();

					template <class... Args>
					iterator emplace(const_iterator position, Args&&... args);
					iterator insert(const_iterator position, const value_type& x);
					iterator insert(const_iterator position, value_type&& x);
					iterator insert(const_iterator position, size_type n, const value_type& x);
					template <class InputIterator>
					iterator insert(const_iterator position, InputIterator first,
						InputIterator last);
					iterator insert(const_iterator position,
						initializer_list<value_type> il);
					iterator erase(const_iterator position);
					iterator erase(const_iterator first, const_iterator last);
					void swap(path_builder&)
						noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value
							|| allocator_traits<Allocator>::is_always_equal::value);
					void clear() noexcept;

					// Observers
					::std::experimental::io2d::rectangle path_extents() const;
				};

				template <class Allocator>
				bool operator==(const path_builder<Allocator>& lhs,
					const path_builder<Allocator>& rhs);
				template <class Allocator>
				bool operator!=(const path_builder<Allocator>& lhs,
					const path_builder<Allocator>& rhs);
				// 8.17.8, specialized algorithms:
				template <class Allocator>
				void swap(path_builder<Allocator>& lhs, path_builder<Allocator>& rhs)
					noexcept(noexcept(lhs.swap(rhs)));

				class device {
				public:
					typedef cairo_device_t* native_handle_type;
				private:
					friend surface;
					explicit device(native_handle_type nh);
					device(native_handle_type nh, error_code& ec) noexcept;
				protected:
					::std::shared_ptr<cairo_device_t> _Device;
				public:
					native_handle_type native_handle() const noexcept;

					device() = delete;
					device(device&& other) noexcept = default;
					device& operator=(device&& other) noexcept = default;

					// Modifiers
					void flush() noexcept;
					void lock();
					void lock(::std::error_code& ec) noexcept;
					void unlock();
					void unlock(::std::error_code& ec) noexcept;
				};

				class color_stop {
				private:
					double _Offset = 0.0;
					bgra_color _Color = bgra_color::black();
				public:
					color_stop() noexcept = default;
					color_stop(double offset, const bgra_color& color);

					void offset(double value) noexcept;
					void color(const bgra_color& value) noexcept;

					double offset() const noexcept;
					bgra_color color() const noexcept;
				};

				class render_props {
					matrix_2d _Matrix;// = matrix_2d::init_identity(); // Transformation matrix
					experimental::io2d::antialias _Antialias = experimental::io2d::antialias::good;
					experimental::io2d::compositing_op _Compositing = experimental::io2d::compositing_op::over;
				public:
					constexpr render_props() noexcept {}
					constexpr explicit render_props(const matrix_2d& m,
						antialias a = antialias::good,
						compositing_op co = compositing_op::over) noexcept
						: _Matrix(m)
						, _Antialias(a)
						, _Compositing(co) {}

					constexpr void antialiasing(antialias a) noexcept {
						_Antialias = a;
					}
					constexpr void compositing(compositing_op co) noexcept {
						_Compositing = co;
					}
					constexpr void surface_matrix(const matrix_2d& m) noexcept {
						_Matrix = m;
					}

					constexpr antialias antialiasing() const noexcept {
						return _Antialias;
					}
					constexpr compositing_op compositing() const noexcept {
						return _Compositing;
					}
					constexpr matrix_2d surface_matrix() const noexcept {
						return _Matrix;
					}
				};

				class clip_props {
					path_group _Clip;
					experimental::io2d::fill_rule _Fill_rule = experimental::io2d::fill_rule::winding;
				public:
					clip_props() noexcept = default;
					template <class Allocator>
					explicit clip_props(const path_builder<Allocator> &pf,
						experimental::io2d::fill_rule fr = experimental::io2d::fill_rule::winding)
						: _Clip(path_group(pf))
						, _Fill_rule(fr) { }

					explicit clip_props(const path_group& pg,
						experimental::io2d::fill_rule fr = experimental::io2d::fill_rule::winding) noexcept
						: _Clip(pg)
						, _Fill_rule(fr) { }

					explicit clip_props(const rectangle& r,
						experimental::io2d::fill_rule fr = experimental::io2d::fill_rule::winding)
						: _Clip()
						, _Fill_rule(fr) {
						path_builder<> clip;
						clip.new_path(r.top_left());
						clip.rel_line_to({ r.width(), 0.0 });
						clip.rel_line_to({ 0.0, r.height() });
						clip.rel_line_to({ -r.width(), 0.0 });
						clip.close_path();
						_Clip = path_group(clip);
					}

					template <class Allocator>
					void clip(const path_builder<Allocator>& pf) {
						_Clip = path_group(pf);
					}

					void clip(const path_group& pg) noexcept {
						_Clip = pg;
					}

					void fill_rule(experimental::io2d::fill_rule fr) {
						_Fill_rule = fr;
					}

					path_group clip() const noexcept {
						return _Clip;
					}

					experimental::io2d::fill_rule fill_rule() const noexcept {
						return _Fill_rule;
					}
				};

				class stroke_props {
					double _Line_width = 2.0;
					double _Miter_limit = 10.0;
					experimental::io2d::line_cap _Line_cap = experimental::io2d::line_cap::none;
					experimental::io2d::line_join _Line_join = experimental::io2d::line_join::miter;
					//optional<dashes> _Dashes;
				public:
					constexpr stroke_props() noexcept {}
					constexpr explicit stroke_props(double w,
						experimental::io2d::line_cap lc = experimental::io2d::line_cap::none,
						experimental::io2d::line_join lj = experimental::io2d::line_join::miter,
						double ml = 10.0) noexcept
						: _Line_width(w)
						, _Miter_limit(ml)
						, _Line_cap(lc)
						, _Line_join(lj) {}

					constexpr void line_width(double w) noexcept {
						_Line_width = w;
					}
					constexpr void line_cap(experimental::io2d::line_cap lc) noexcept {
						_Line_cap = lc;
					}
					constexpr void line_join(experimental::io2d::line_join lj) noexcept {
						_Line_join = lj;
					}
					constexpr void miter_limit(double ml) noexcept {
						_Miter_limit = ml;
					}

					constexpr double line_width() const noexcept {
						return _Line_width;
					}
					constexpr experimental::io2d::line_cap line_cap() const noexcept {
						return _Line_cap;
					}
					constexpr experimental::io2d::line_join line_join() const noexcept {
						return _Line_join;
					}
					constexpr double miter_limit() const noexcept {
						return _Miter_limit;
					}
				};

				class brush_props {
					experimental::io2d::wrap_mode _Wrap_mode = experimental::io2d::wrap_mode::none;
					experimental::io2d::filter _Filter = experimental::io2d::filter::good;
					experimental::io2d::fill_rule _Fill_rule = experimental::io2d::fill_rule::winding;
					matrix_2d _Matrix = matrix_2d{};

				public:
					constexpr brush_props(experimental::io2d::wrap_mode w = experimental::io2d::wrap_mode::none,
						experimental::io2d::filter fi = experimental::io2d::filter::good,
						experimental::io2d::fill_rule fr = experimental::io2d::fill_rule::winding,
						matrix_2d m = matrix_2d{}) noexcept
						: _Wrap_mode(w)
						, _Filter(fi)
						, _Fill_rule(fr)
						, _Matrix(m) {
					}

					constexpr void wrap_mode(experimental::io2d::wrap_mode w) noexcept {
						_Wrap_mode = w;
					}
					constexpr void filter(experimental::io2d::filter fi) noexcept {
						_Filter = fi;
					}
					constexpr void fill_rule(experimental::io2d::fill_rule fr) noexcept {
						_Fill_rule = fr;
					}
					constexpr void matrix(const matrix_2d& m) noexcept {
						_Matrix = m;
					}

					constexpr experimental::io2d::wrap_mode wrap_mode() const noexcept {
						return _Wrap_mode;
					}
					constexpr experimental::io2d::filter filter() const noexcept {
						return _Filter;
					}
					constexpr experimental::io2d::fill_rule fill_rule() const noexcept {
						return _Fill_rule;
					}
					constexpr matrix_2d matrix() const noexcept {
						return _Matrix;
					}
				};

				class mask_props {
					experimental::io2d::wrap_mode _Wrap_mode = experimental::io2d::wrap_mode::repeat;
					experimental::io2d::filter _Filter = experimental::io2d::filter::good;
					matrix_2d _Matrix = matrix_2d{};

				public:
					constexpr mask_props(experimental::io2d::wrap_mode w = experimental::io2d::wrap_mode::repeat,
						experimental::io2d::filter fi = experimental::io2d::filter::good,
						matrix_2d m = matrix_2d{}) noexcept
						: _Wrap_mode(w)
						, _Filter(fi)
						, _Matrix(m) {
					}
					constexpr mask_props(const mask_props&) noexcept = default;
					constexpr mask_props& operator=(const mask_props&) noexcept = default;
					mask_props(mask_props&&) noexcept = default;
					mask_props& operator=(mask_props&&) noexcept = default;

					constexpr void wrap_mode(experimental::io2d::wrap_mode w) noexcept {
						_Wrap_mode = w;
					}
					constexpr void filter(experimental::io2d::filter fi) noexcept {
						_Filter = fi;
					}
					constexpr void matrix(const matrix_2d& m) noexcept {
						_Matrix = m;
					}

					constexpr experimental::io2d::wrap_mode wrap_mode() const noexcept {
						return _Wrap_mode;
					}
					constexpr experimental::io2d::filter filter() const noexcept {
						return _Filter;
					}
					constexpr matrix_2d matrix() const noexcept {
						return _Matrix;
					}
				};

				class image_surface;

				class brush {
				public:
					typedef cairo_pattern_t* native_handle_type;

				private:
					// Precondition: nh has already had its reference count incremented (either in creation or with cairo_pattern_reference).
					brush(native_handle_type nh) noexcept;

					::std::shared_ptr<cairo_pattern_t> _Brush;
					::std::shared_ptr<image_surface> _Image_surface;
					brush_type _Brush_type;

				public:
					native_handle_type native_handle() const noexcept;

					explicit brush(const bgra_color& c);

					template <class InputIterator>
					brush(const vector_2d& begin, const vector_2d& end,
						InputIterator first, InputIterator last);
					brush(const vector_2d& begin, const vector_2d& end,
						::std::initializer_list<color_stop> il);

					template <class InputIterator>
					brush(const circle& start, const circle& end,
						InputIterator first, InputIterator last);

					brush(const circle& start, const circle& end,
						::std::initializer_list<color_stop> il);

					explicit brush(image_surface&& img);

					brush_type type() const noexcept;
				};

				struct _Surface_native_handles {
					::cairo_surface_t* csfce;
					::cairo_t* cctxt;
				};

				class mapped_surface;

				//tuple<dashes, offset>
				typedef ::std::tuple<::std::vector<double>, double> dashes;

				class surface {
				public:
					typedef _Surface_native_handles native_handle_type;
				private:
					::std::weak_ptr<::std::experimental::io2d::device> _Device;
				protected:
					::std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> _Surface;
					::std::unique_ptr<cairo_t, decltype(&cairo_destroy)> _Context;

					const double _Line_join_miter_miter_limit = 10000.0;

					rectangle _Dirty_rect;
					::std::experimental::io2d::format _Format;

					surface(::std::experimental::io2d::format fmt, int width, int height);

					surface(native_handle_type nh, ::std::experimental::io2d::format fmt);
				protected:
					surface(surface&& other) noexcept = default;
					surface& operator=(surface&& other) noexcept = default;

				public:
					native_handle_type native_handle() const;

					// \ref{\iotwod.surface.cons}, constructors:
					surface() = delete;

					// \ref{\iotwod.surface.modifiers.state}, state modifiers:
					void flush();
					void flush(::std::error_code& ec) noexcept;
					::std::shared_ptr<experimental::io2d::device> device();
					::std::shared_ptr<experimental::io2d::device> device(::std::error_code& ec) noexcept;
					void mark_dirty();
					void mark_dirty(const rectangle& rect);
					void map(const ::std::function<void(mapped_surface&)>& action);
					void map(const ::std::function<void(mapped_surface&, error_code&)>& action, ::std::error_code& ec);
					void map(const ::std::function<void(mapped_surface&)>& action, const rectangle& extents);
					void map(const ::std::function<void(mapped_surface&, error_code&)>& action, const rectangle& extents, ::std::error_code& ec);

					// \ref{\iotwod.surface.modifiers.render}, render modifiers:
					void clear();
					void paint(const brush& b, const optional<brush_props>& bp = nullopt, const optional<render_props>& rp = nullopt, const optional<clip_props>& cl = nullopt);
					template <class Allocator>
					void fill(const brush& b, const path_builder<Allocator>& pf, const optional<brush_props>& bp = nullopt, const optional<render_props>& rp = nullopt, const optional<clip_props>& cl = nullopt) {
						path_group pg(pf);
						fill(b, pg, bp, rp, cl);
					}
					void fill(const brush& b, const path_group& pg, const optional<brush_props>& bp = nullopt, const optional<render_props>& rp = nullopt, const optional<clip_props>& cl = nullopt);
					template <class Allocator>
					void stroke(const brush& b, const path_builder<Allocator>& pf, const optional<brush_props>& bp = nullopt, const optional<stroke_props>& sp = nullopt, const optional<dashes>& d = nullopt, const optional<render_props>& rp = nullopt, const optional<clip_props>& cl = nullopt) {
						path_group pg(pf);
						stroke(b, pg, bp, sp, d, rp, cl);
					}
					void stroke(const brush& b, const path_group& pg, const optional<brush_props>& bp = nullopt, const optional<stroke_props>& sp = nullopt, const optional<dashes>& d = nullopt, const optional<render_props>& rp = nullopt, const optional<clip_props>& cl = nullopt);
					template <class Allocator>
					void mask(const brush& b, const brush& mb, const path_builder<Allocator>& pf, const optional<brush_props>& bp = nullopt, const optional<mask_props>& mp = nullopt, const optional<render_props>&rp = nullopt, const optional<clip_props>& cl = nullopt) {
						path_group pg(pf);
						mask(b, mb, pg, bp, mp, rp, cl);
					}
					void mask(const brush& b, const brush& mb, const path_group& pg, const optional<brush_props>& bp = nullopt, const optional<mask_props>& mp = nullopt, const optional<render_props>& rp = nullopt, const optional<clip_props>& cl = nullopt);
				};

				enum class image_data_format {
					png,
					jpg
				};

				class image_surface : public surface {
					friend surface;
				public:
					image_surface(image_surface&& other) /*noexcept*/ = default;
					image_surface& operator=(image_surface&& other) /*noexcept*/ = default;
					image_surface(::std::experimental::io2d::format fmt, int width, int height);
#ifdef _Filesystem_support_test
					image_surface(::std::experimental::filesystem::path f, experimental::io2d::format fmt, image_data_format idf);
#else
					image_surface(::std::string f, experimental::io2d::format fmt, image_data_format idf);
#endif
					// Modifiers
#ifdef _Filesystem_support_test
					void save(::std::experimental::filesystem::path f, image_data_format idf);
#else
					void save(::std::string f, image_data_format idf);
#endif
					// Observers
					::std::experimental::io2d::format format() const noexcept;
					int width() const noexcept;
					int height() const noexcept;
					int stride() const noexcept;
				};

				class mapped_surface {
					surface::native_handle_type _Mapped_surface;
					surface::native_handle_type _Map_of;

					friend surface;
					mapped_surface(surface::native_handle_type nh, surface::native_handle_type map_of);
					mapped_surface(surface::native_handle_type nh, surface::native_handle_type map_of, ::std::error_code& ec) noexcept;

				public:
					mapped_surface() = delete;
					mapped_surface(const mapped_surface&) = delete;
					mapped_surface& operator=(const mapped_surface&) = delete;
					mapped_surface(mapped_surface&& other) = delete;
					mapped_surface& operator=(mapped_surface&& other) = delete;
					~mapped_surface();

					// Modifiers
					void commit_changes();
					void commit_changes(::std::error_code& ec) noexcept;
					void commit_changes(const rectangle& area);
					void commit_changes(const rectangle& area, ::std::error_code& ec) noexcept;

					unsigned char* data();
					unsigned char* data(::std::error_code& ec) noexcept;

					// Observers
					const unsigned char* data() const;
					const unsigned char* data(::std::error_code& ec) const noexcept;

					::std::experimental::io2d::format format() const noexcept;
					int width() const noexcept;
					int height() const noexcept;
					int stride() const noexcept;
				};

#ifdef _WIN32_WINNT
				struct _Win32_display_surface_native_handle {
					_Surface_native_handles sfc_nh;
					HWND hwnd;
					_Surface_native_handles win32_sfc_nh;
				};

				const int _Display_surface_ptr_window_data_byte_offset = 0;

				LRESULT CALLBACK _RefImplWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#elif defined(USE_XCB)
				struct _Xcb_display_surface_native_handle {
					_Surface_native_handles sfc_nh;
					_Surface_native_handles display_sfc_nh;
					xcb_connection_t* connection;
					::std::mutex& connection_mutex;
					int& connection_ref_count;
					xcb_screen_t* screen;
					xcb_window_t wndw;
				};
#elif defined(USE_XLIB)
				struct _Xlib_display_surface_native_handle {
					_Surface_native_handles sfc_nh;
					Display* display;
					Window wndw;
					::std::mutex& display_mutex;
					int& display_ref_count;
				};
#endif

				class display_surface : public surface {
					friend surface;

					::std::experimental::io2d::brush _Default_brush;
					//brush_props _Default_brush_props; // Unneeded since it's a solid color brush.
					int _Display_width;
					int _Display_height;
					::std::experimental::io2d::scaling _Scaling;
					int _Width;
					int _Height;
					::std::unique_ptr<::std::function<void(display_surface& sfc)>> _Draw_fn;
					::std::unique_ptr<::std::function<void(display_surface& sfc)>> _Size_change_fn;
					::std::unique_ptr<::std::function<::std::experimental::io2d::rectangle(const display_surface&, bool&)>> _User_scaling_fn;
					optional<experimental::io2d::brush> _Letterbox_brush;
					optional<brush_props> _Letterbox_brush_props;
					typedef bool _Auto_clear_type;
					bool _Auto_clear;

#ifdef _WIN32_WINNT
					friend LRESULT CALLBACK _RefImplWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
					DWORD _Window_style;
					HWND _Hwnd;

					LRESULT _Window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#elif defined(USE_XCB)
					static ::std::mutex _Connection_mutex;
					static ::std::unique_ptr<xcb_connection_t, decltype(&xcb_disconnect)> _Connection;
					static int _Connection_ref_count;
					xcb_screen_t* _Screen;
					xcb_window_t _Wndw;
					bool _Can_draw = false;
					const uint16_t _Window_border_width = 4;
#elif defined(USE_XLIB)
					static ::std::mutex _Display_mutex;
					static ::std::unique_ptr<Display, ::std::function<void(Display*)>> _Display;
					static int _Display_ref_count;
					::Window _Wndw;
					bool _Can_draw = false;

					static Bool _X11_if_event_pred(Display* display, XEvent* event, XPointer arg);
#endif
					::std::experimental::io2d::refresh_rate _Refresh_rate;
					double _Desired_frame_rate;
					bool _Redraw_requested;
					double _Elapsed_draw_time;
					const double _Minimum_frame_rate = 0.01;
					const double _Maximum_frame_rate = 120.0;
					::std::unique_ptr<cairo_surface_t, ::std::function<void(cairo_surface_t*)>> _Native_surface;
					::std::unique_ptr<cairo_t, ::std::function<void(cairo_t*)>> _Native_context;

					void _Make_native_surface_and_context();
					void _Make_native_surface_and_context(::std::error_code& ec) noexcept;
					void _All_dimensions(int w, int h, int dw, int dh);
					void _All_dimensions(int w, int h, int dw, int dh, ::std::error_code& ec) noexcept;
					void _Render_to_native_surface();
					void _Render_to_native_surface(::std::error_code& ec) noexcept;
					void _Resize_window();
					void _Resize_window(::std::error_code& ec) noexcept;
					void _Render_for_scaling_uniform_or_letterbox();
					void _Render_for_scaling_uniform_or_letterbox(::std::error_code& ec) noexcept;

				public:
#ifdef _WIN32_WINNT
					typedef _Win32_display_surface_native_handle native_handle_type;
#elif defined(USE_XCB)
					typedef _Xcb_display_surface_native_handle native_handle_type;
#elif defined(USE_XLIB)
					typedef _Xlib_display_surface_native_handle native_handle_type;
#endif
					native_handle_type native_handle() const;

					//display_surface() = delete;
					//display_surface(const display_surface&) = delete;
					//display_surface& operator=(const display_surface&) = delete;
					display_surface(display_surface&& other) /*noexcept*/ = default;
					display_surface& operator=(display_surface&& other) /*noexcept*/ = default;

					display_surface(int preferredWidth, int preferredHeight, ::std::experimental::io2d::format preferredFormat,
						::std::experimental::io2d::scaling scl = ::std::experimental::io2d::scaling::letterbox, ::std::experimental::io2d::refresh_rate rr = ::std::experimental::io2d::refresh_rate::as_fast_as_possible, double fps = 30.0);
					//display_surface(int preferredWidth, int preferredHeight, ::std::experimental::io2d::format preferredFormat, ::std::error_code& ec,
					//	::std::experimental::io2d::scaling scl = ::std::experimental::io2d::scaling::letterbox, ::std::experimental::io2d::refresh_rate rr = ::std::experimental::io2d::refresh_rate::as_fast_as_possible, double fps = 30.0) noexcept;

					display_surface(int preferredWidth, int preferredHeight, ::std::experimental::io2d::format preferredFormat,
						int preferredDisplayWidth, int preferredDisplayHeight,
						::std::experimental::io2d::scaling scl = ::std::experimental::io2d::scaling::letterbox, ::std::experimental::io2d::refresh_rate rr = ::std::experimental::io2d::refresh_rate::as_fast_as_possible, double fps = 30.0);

					~display_surface();

					void draw_callback(const ::std::function<void(display_surface& sfc)>& fn);
					void size_change_callback(const ::std::function<void(display_surface& sfc)>& fn);
					void width(int w) noexcept;
					void height(int h) noexcept;
					void display_width(int w) noexcept;
					void display_height(int h) noexcept;
					void dimensions(int w, int h) noexcept;
					void display_dimensions(int dw, int dh) noexcept;
					void scaling(experimental::io2d::scaling scl) noexcept;
					void user_scaling_callback(const ::std::function<::std::experimental::io2d::rectangle(const display_surface&, bool&)>& fn);
					void letterbox_brush(const optional<brush>& b, const optional<brush_props>& bp = nullopt) noexcept;
					void auto_clear(bool val) noexcept;
					void refresh_rate(::std::experimental::io2d::refresh_rate rr) noexcept;
					bool desired_frame_rate(double fps) noexcept;
					void redraw_required() noexcept;

					int begin_show();
					void end_show() noexcept;

					experimental::io2d::format format() const noexcept;
					int width() const noexcept;
					int height() const noexcept;
					int display_width() const noexcept;
					int display_height() const noexcept;
					vector_2d dimensions() const noexcept;
					vector_2d display_dimensions() const noexcept;
					experimental::io2d::scaling scaling() const noexcept;
					function<experimental::io2d::rectangle(const display_surface&, bool&)> user_scaling_callback() const;
					function<experimental::io2d::rectangle(const display_surface&, bool&)> user_scaling_callback(::std::error_code& ec) const noexcept;
					const optional<brush>& letterbox_brush() const noexcept;
					optional<brush_props> letterbox_brush_props() const noexcept;
					bool auto_clear() const noexcept;
					experimental::io2d::refresh_rate refresh_rate() const noexcept;
					double desired_frame_rate() const noexcept;
					double elapsed_draw_time() const noexcept;
				};

#ifdef _MSC_VER
			}
		}
	}
}
#include <ios>
#include <ostream>
#include <sstream>
namespace std {
	template <class Traits>
	basic_ostream<char, Traits>& operator<<(basic_ostream<char, Traits>& os, const ::std::experimental::io2d::vector_2d& v);
	template <class Traits>
	basic_ostream<char, Traits>& operator<<(basic_ostream<char, Traits>& os, const ::std::experimental::io2d::vector_2d& v) {
		fixed(os);
		os << "vector_2d(x y)\t" << v.x() << "\t" << v.y() << "\t";
		return os;
	}
}
namespace std {
	namespace experimental {
		namespace io2d {
			inline namespace v1 {
#endif
				int format_stride_for_width(format format, int width) noexcept;
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0);
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, ::std::error_code& ec, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0) noexcept;
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, int preferredDisplayWidth, int preferredDisplayHeight, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0);
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat,
					int preferredDisplayWidth, int preferredDisplayHeight, ::std::error_code& ec, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0) noexcept;
				image_surface make_image_surface(format format, int width, int height);
				image_surface make_image_surface(format format, int width, int height, ::std::error_code& ec) noexcept;
				image_surface make_image_surface(image_surface& sfc);
				double angle_for_point(const vector_2d& ctr, const vector_2d& pt, const vector_2d& scl = vector_2d{ 1.0, 1.0 }) noexcept;
				vector_2d point_for_angle(double ang, double mgn = 1.0) noexcept;

				template <class T, enable_if_t<is_arithmetic_v<T>, int*> = 0>
				constexpr double to_radians(T deg) noexcept {
					const auto angle = static_cast<double>(deg) / 360.0 * two_pi<double>;
					const double oneThousandthOfADegreeInRads = pi<double> / 180'000.0;
					if (((angle > 0.0) && (angle < oneThousandthOfADegreeInRads)) || ((angle < 0.0) && (angle < -oneThousandthOfADegreeInRads))) {
						return (angle < 0.0) ? - 0.0 : 0.0;
					}
					return angle;
				}

				template <class T, enable_if_t<is_arithmetic_v<T>, int*> = 0>
				constexpr double to_degrees(T rad) noexcept {
					const auto angle = static_cast<double>(rad) / two_pi<double> * 360.0;
					const double oneThousandthOfADegree = 0.001;
					if (((angle > 0.0) && (angle < oneThousandthOfADegree)) || ((angle < 0.0) && (angle < -oneThousandthOfADegree))) {
						return (angle < 0.0) ? -0.0 : 0.0;
					}
					return angle;
				}

				template <class _TItem>
				struct _Path_factory_process_visit {
					constexpr static double twoThirds = 2.0 / 3.0;

					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_new_path>, _Path_data_abs_new_path> = _Path_data_abs_new_path_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
						currentPoint = item.at();
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_new_path>, pt);
						closePoint = pt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_new_path>, _Path_data_rel_new_path> = _Path_data_rel_new_path_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
						currentPoint = currentPoint + item.at();
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_new_path>, pt);
						closePoint = pt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::close_path>, _Path_data_close_path> = _Path_data_close_path_val>
					static void _Perform(const T&, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
						v.emplace_back(::std::in_place_type<path_data::close_path>, closePoint);
						v.emplace_back(::std::in_place_type<path_data::abs_new_path>,
							closePoint);
						if (!m.is_finite() || !m.is_invertible()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_matrix));
						}
						auto invM = matrix_2d{ m }.invert();
						// Need to assign the untransformed closePoint value to currentPoint.
						currentPoint = invM.transform_point(closePoint - origin) + origin;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_matrix>, _Path_data_change_matrix> = _Path_data_change_matrix_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d& m, vector_2d&, vector_2d&, vector_2d&) {
						if (!m.is_finite()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_matrix));
						}
						if (!m.is_invertible()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_matrix));
						}
						m = item.matrix();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_origin>, _Path_data_change_origin> = _Path_data_change_origin_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d& origin, vector_2d&, vector_2d&) {
						origin = item.origin();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_cubic_curve>, _Path_data_abs_cubic_curve> = _Path_data_abs_cubic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
						auto pt1 = m.transform_point(item.control_point_1() - origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() - origin) + origin;
						auto pt3 = m.transform_point(item.end_point() - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, pt1,
							pt2, pt3);
						currentPoint = item.end_point();
						closePoint = pt3;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_line>, _Path_data_abs_line> = _Path_data_abs_line_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
						currentPoint = item.to();
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
						closePoint = pt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_quadratic_curve>, _Path_data_abs_quadratic_curve> = _Path_data_abs_quadratic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						//vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() - origin) + origin;
						auto endPt = m.transform_point(item.end_point() - origin) + origin;
						auto beginPt = m.transform_point(currentPoint - origin) + origin;
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y() - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y() - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point();
						closePoint = endPt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc>, _Path_data_arc> = _Path_data_arc_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&) {
						{
							const double rot = item.rotation();
							const double oneThousandthOfADegreeInRads = pi<double> / 180'000.0;
							if (abs(rot) < oneThousandthOfADegreeInRads) {
								// Return if the rotation is less than one thousandth of one degree; it's a degenerate path segment.
								return;
							}
							const auto clockwise = (rot < 0.0) ? true : false;
							const auto quarterpi = half_pi<double> / 2.0;
							const vector_2d rad = item.radius();
							auto startAng = item.start_angle();
							const auto origM = m;
							//m.scale({ rad.x() / rad.y(), 1.0 });
							m.scale(rad);
							const auto origOrigin = origin;
							auto centerOffset = (point_for_angle(two_pi<double> - startAng) * rad);
							auto ctr = currentPoint - centerOffset;
							// Maybe make the origin = origM.transform_point(currentPoint - origin) + origin; Then m.translate(-centerOffset); ? That would transform every point relative to the currentPoint (as actually interpreted). Also, should currentPoint below be an inverse matrix transform of cpt3? The answer is yes.

							origin = ctr;

							//ctr.y(-ctr.y());
							//auto invM = m;
							//invM.invert();
							//auto testPt = invM.transform_point(ctr);
							//bool x = true;
							//if (x) {
							//	origin = ctr;
							//}
							//else {
							//	origin = testPt;
							//}
							//const auto rad = ((ctr - currentPoint) / scl).magnitude();
							//const auto rad = sqrt((ctr - currentPoint).magnitude_squared()/scl);
							//auto startAng = angle_for_point(ctr, currentPoint, scl);
							//auto radDiff = ctr - currentPoint;
							//auto radMag = radDiff.magnitude();
							//auto radCos = cos(startAng);
							//auto radSin = sin(startAng);
							//auto radRot = vector_2d{ radDiff.x() * radCos + radDiff.y() * (radSin), -(radDiff.x() * (-radSin) + radDiff.y() * radCos) };
							//auto rad = radRot.magnitude();

							vector_2d pt0, pt1, pt2, pt3;
							int bezCount = 1;
							double theta = rot;// abs(rot);

							//while (theta > half_pi<double>) {
							while (abs(theta) > half_pi<double>) {
								//while (theta > quarterpi) {
								theta /= 2.0;
								bezCount += bezCount;
							}
							double phi;
							if (clockwise) {
								phi = (theta / 2.0);
								const auto cosPhi = cos(-phi);
								const auto sinPhi = sin(-phi);
								pt0.x(cosPhi);
								pt0.y(-sinPhi);
								pt3.x(pt0.x());
								pt3.y(-pt0.y());
								pt1.x((4.0 - cosPhi) / 3.0);
								pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
								pt2.x(pt1.x());
								pt2.y(-pt1.y());
							}
							else {
								phi = (theta / 2.0);
								const auto cosPhi = cos(phi);
								const auto sinPhi = sin(phi);
								pt0.x(cosPhi);
								pt0.y(-sinPhi);
								pt3.x(pt0.x());
								pt3.y(-pt0.y());
								pt1.x((4.0 - cosPhi) / 3.0);
								pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
								pt2.x(pt1.x());
								pt2.y(-pt1.y());
							}
							auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
								//auto result = vector_2d{ pt.x() * cos(a) + pt.y() * sin(a),
								//	-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
								auto result = vector_2d{ pt.x() * cos(a) - pt.y() * sin(a),
									pt.x() * sin(a) + pt.y() * cos(a) };
								if (abs(result.x()) < numeric_limits<double>::epsilon() * 100.0) {
									result.x(result.x() < 0.0 ? -0.0 : 0.0);
								}
								if (abs(result.y()) < numeric_limits<double>::epsilon() * 100.0) {
									result.y(result.y() < 0.0 ? -0.0 : 0.0);
								}
								return result;
							};
#ifdef _MSC_VER
							stringstream outputDebugStr;
							outputDebugStr.str(string());
							outputDebugStr << "is clockwise:\t" << boolalpha << clockwise << "\ntheta:\t" << theta << "\nphi:\t" << phi << "\nstartAng:\t" << startAng << "\nrotation:\t" << rot << "\nend angle:\t" << startAng + rot << "\npt0:\t" << pt0 << "\npt1:\t" << pt1 << "\npt2:\t" << pt2 << "\npt3:\t" << pt3 << "\nbezCount:\t" << bezCount << "\nradius:\t" << rad << "\ncenter:\t" << ctr << "\n";
							OutputDebugStringA(outputDebugStr.str().c_str());
#endif

							const auto twopi = two_pi<double>;
							startAng = two_pi<double> -startAng;

							if (clockwise) {
								pt0 = rotCwFn(pt0, phi);
								pt1 = rotCwFn(pt1, phi);
								pt2 = rotCwFn(pt2, phi);
								pt3 = rotCwFn(pt3, phi);
								pt0.y(-pt0.y());
								pt1.y(-pt1.y());
								pt2.y(-pt2.y());
								pt3.y(-pt3.y());
								auto shflPt = pt3;
								pt3 = pt0;
								pt0 = shflPt;
								shflPt = pt2;
								pt2 = pt1;
								pt1 = shflPt;
							}
							else {
								auto shflPt = pt0;
								pt0 = rotCwFn(pt3, phi);
								pt3 = rotCwFn(shflPt, phi);
								shflPt = pt1;
								pt1 = rotCwFn(pt2, phi);
								pt2 = rotCwFn(pt1, phi);
								//pt0 = rotCwFn(pt0, phi);
								//pt1 = rotCwFn(pt1, phi);
								//pt2 = rotCwFn(pt2, phi);
								//pt3 = rotCwFn(pt3, phi);
								//auto shflPt = pt3;
								//pt3 = pt0;
								//pt0 = shflPt;
								//shflPt = pt2;
								//pt2 = pt1;
								//pt1 = shflPt;
							}
							auto currTheta = startAng;
							//const auto apt0 = (pt0 * rad);
							//const auto apt1 = (pt1 * rad);
							//const auto apt2 = (pt2 * rad);
							//const auto apt3 = (pt3 * rad);
#ifdef _MSC_VER
							outputDebugStr.str(string());
							outputDebugStr << "pt0:\t" << pt0 << "\npt1:\t" << pt1 << "\npt2:\t" << pt2 << "\npt3:\t" << pt3 <</* "\napt0:\t" << apt0 << "\napt1:\t" << apt1 << "\napt2:\t" << apt2 << "\napt3:\t" << apt3 << "\ncurrTheta:\t" << currTheta <<*/ "\n\n";
							OutputDebugStringA(outputDebugStr.str().c_str());
#endif
							
							//const auto oneRad = (rad.x() > rad.y()) ? rad.x() : rad.y();
							const auto oneRad = 1.0;
							for (; bezCount > 0; bezCount--) {
								////const auto rapt0 = rotCwFn(apt0, currTheta);
								////const auto rapt1 = rotCwFn(apt1, currTheta);
								////const auto rapt2 = rotCwFn(apt2, currTheta);
								////const auto rapt3 = rotCwFn(apt3, currTheta);
								//const auto rapt0 = rotCwFn(pt0, currTheta) * oneRad;
								//const auto rapt1 = rotCwFn(pt1, currTheta) * oneRad;
								//const auto rapt2 = rotCwFn(pt2, currTheta) * oneRad;
								//const auto rapt3 = rotCwFn(pt3, currTheta) * oneRad;
								//auto cpt0 = ctr + rapt3;
								//auto cpt1 = ctr + rapt2;
								//auto cpt2 = ctr + rapt1;
								//auto cpt3 = ctr + rapt0;
								////auto cpt1 = ctr + rotCwFn(apt1, currTheta);
								////auto cpt2 = ctr + rotCwFn(apt2, currTheta);
								////auto cpt3 = ctr + rotCwFn(apt3, currTheta);
								//const auto rapt0 = rotCwFn(m.transform_point(pt0), currTheta);
								//const auto rapt1 = rotCwFn(m.transform_point(pt1), currTheta);
								//const auto rapt2 = rotCwFn(m.transform_point(pt2), currTheta);
								//const auto rapt3 = rotCwFn(m.transform_point(pt3), currTheta);
								const auto rapt0 = m.transform_point(rotCwFn(pt0, currTheta));
								const auto rapt1 = m.transform_point(rotCwFn(pt1, currTheta));
								const auto rapt2 = m.transform_point(rotCwFn(pt2, currTheta));
								const auto rapt3 = m.transform_point(rotCwFn(pt3, currTheta));
								auto cpt0 = ctr + rapt0;
								auto cpt1 = ctr + rapt1;
								auto cpt2 = ctr + rapt2;
								auto cpt3 = ctr + rapt3;
#ifdef _MSC_VER
								outputDebugStr.str(string());
								outputDebugStr << "currentPoint:\t" << currentPoint << "\ncurrTheta:\t" << currTheta << "\ntheta:\t" << theta << "\noneRad:\t" << oneRad << "\nrapt0:\t" << rapt0 << "\nrapt1:\t" << rapt1 << "\nrapt2:\t" << rapt2 << "\nrapt3:\t" << rapt3 << "\ncpt0:\t" << cpt0 << "\ncpt1:\t" << cpt1 << "\ncpt2:\t" << cpt2 << "\ncpt3:\t" << cpt3 << "\n";
								OutputDebugStringA(outputDebugStr.str().c_str());
#endif
//								//cpt1 = ctr + point_for_angle(pt1.angular_direction() + currTheta) * rad;
//								//cpt2 = ctr + point_for_angle(pt2.angular_direction() + currTheta) * rad;
//								//cpt3 = ctr + point_for_angle(pt3.angular_direction() + currTheta) * rad;
//								cpt0 = m.transform_point(cpt0 - origin) + origin;
//								cpt1 = m.transform_point(cpt1 - origin) + origin;
//								cpt2 = m.transform_point(cpt2 - origin) + origin;
//								cpt3 = m.transform_point(cpt3 - origin) + origin;
#ifdef _MSC_VER
								outputDebugStr.str(string());
								outputDebugStr << "\ncpt0:\t" << cpt0 << "\ncpt1:\t" << cpt1 << "\ncpt2:\t" << cpt2 << "\ncpt3:\t" << cpt3 << "\n\n";
								OutputDebugStringA(outputDebugStr.str().c_str());
#endif
//								auto inverseM = m;
//								m.invert();
//								currentPoint = inverseM.transform_point(cpt3 - origin) + origin;
								currentPoint = cpt3;
								v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, cpt3);
								if (clockwise) {
									currTheta -= theta;
								}
								else {
									currTheta -= theta;
								}
							}
#ifdef _MSC_VER
							outputDebugStr.str(string());
							outputDebugStr << "currentPoint:\t" << currentPoint << "\ncenter:\t" << ctr << "\n\n'--------------\t'--------------\t'--------------\t'--------------\n\n";
							OutputDebugStringA(outputDebugStr.str().c_str());
#endif
							origin = origOrigin;
							m = origM;
						}
					}
					//template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_arc_clockwise>, _Path_data_rel_arc_clockwise> = _Path_data_rel_arc_clockwise_val>
					//static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
					//	path_data::abs_arc_clockwise absItem{};
					//	absItem.center(currentPoint + item.center());
					//	absItem.end_angle(item.end_angle());
					//	absItem.scale(item.scale());
					//	_Path_factory_process_visit::template _Perform(absItem, v, m, origin, currentPoint, closePoint);
					//}
					//template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_arc_counterclockwise>, _Path_data_abs_arc_counterclockwise> = _Path_data_abs_arc_counterclockwise_val>
					//static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&) {
					//	{
					//		const auto endAngRot = item.end_angle();
					//		const double oneThousandthOfADegreeInRads = pi<double> / 180'000.0;
					//		if (abs(endAngRot) < oneThousandthOfADegreeInRads) {
					//			// Return if the rotation is less than one thousandth of one degree; it's a degenerate path segment.
					//			return;
					//		}
					//		const auto ctr = item.center();
					//		const auto scl = item.scale();
					//		const auto origM = m;
					//		m.scale(scl);
					//		const auto origOrigin = origin;
					//		origin = ctr;
					//		const auto rad = ((ctr - currentPoint) / scl).magnitude();
					//		auto startAng = angle_for_point(ctr, currentPoint, scl);
					//		vector_2d pt0, pt1, pt2, pt3;
					//		int bezCount = 1;
					//		double theta = endAngRot;
					//		while (theta >= half_pi<double>) {
					//			theta /= 2.0;
					//			bezCount += bezCount;
					//		}
					//		const double phi = theta / 2.0;
					//		const auto cosPhi = cos(phi);
					//		const auto sinPhi = sin(phi);
					//		pt0.x(cosPhi);
					//		pt0.y(-sinPhi);
					//		pt3.x(pt0.x());
					//		pt3.y(-pt0.y());
					//		pt1.x((4.0 - cosPhi) / 3.0);
					//		pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
					//		pt2.x(pt1.x());
					//		pt2.y(-pt1.y());

					//		auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
					//			auto result = vector_2d{ pt.x() * cos(a) + pt.y() * sin(a),
					//				-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
					//			if (abs(result.x()) < numeric_limits<double>::epsilon() * 100.0) {
					//				result.x(result.x() < 0 ? -0.0 : 0.0);
					//			}
					//			if (abs(result.y()) < numeric_limits<double>::epsilon() * 100.0) {
					//				result.y(result.y() < 0 ? -0.0 : 0.0);
					//			}
					//			return result;
					//		};

					//		startAng = two_pi<double> -startAng;
					//		pt0 = rotCwFn(pt0, phi);
					//		pt1 = rotCwFn(pt1, phi);
					//		pt2 = rotCwFn(pt2, phi);
					//		pt3 = rotCwFn(pt3, phi);
					//		auto shflPt = pt3;
					//		pt3 = pt0;
					//		pt0 = shflPt;
					//		shflPt = pt2;
					//		pt2 = pt1;
					//		pt1 = shflPt;

					//		auto currTheta = startAng;
					//		const auto apt1 = (pt1 * rad);
					//		const auto apt2 = (pt2 * rad);
					//		const auto apt3 = (pt3 * rad);

					//		for (; bezCount > 0; bezCount--) {
					//			auto cpt1 = ctr + rotCwFn(apt1, currTheta);
					//			auto cpt2 = ctr + rotCwFn(apt2, currTheta);
					//			auto cpt3 = ctr + rotCwFn(apt3, currTheta);
					//			cpt1 = m.transform_point(cpt1 - origin) + origin;
					//			cpt2 = m.transform_point(cpt2 - origin) + origin;
					//			cpt3 = m.transform_point(cpt3 - origin) + origin;
					//			currentPoint = cpt3;
					//			v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, cpt3);
					//			currTheta -= theta;
					//		}
					//		origin = origOrigin;
					//		m = origM;
					//	}
					//}
					//template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_arc_counterclockwise>, _Path_data_rel_arc_counterclockwise> = _Path_data_rel_arc_counterclockwise_val>
					//static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint) {
					//	path_data::abs_arc_counterclockwise absItem{};
					//	absItem.center(currentPoint + item.center());
					//	absItem.end_angle(item.end_angle());
					//	absItem.scale(item.scale());
					//	_Path_factory_process_visit::template _Perform(absItem, v, m, origin, currentPoint, closePoint);
					//}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_cubic_curve>, _Path_data_rel_cubic_curve> = _Path_data_rel_cubic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&) {
						auto pt1 = m.transform_point(item.control_point_1() + currentPoint -
							origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() + currentPoint -
							origin) + origin;
						auto pt3 = m.transform_point(item.end_point() + currentPoint - origin) +
							origin;
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>,
							pt1, pt2, pt3);
						currentPoint = item.end_point() + currentPoint;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_line>, _Path_data_rel_line> = _Path_data_rel_line_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&) {
						currentPoint = item.to() + currentPoint;
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_quadratic_curve>, _Path_data_rel_quadratic_curve> = _Path_data_rel_quadratic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&) {
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() + currentPoint -
							origin) + origin;
						auto endPt = m.transform_point(item.end_point() + currentPoint -
							origin) + origin;
						beginPt = m.transform_point(currentPoint - origin) + origin;
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y() - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y() - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point() + currentPoint;
					}
				};

				template <class Allocator>
				inline ::std::vector<path_data::path_data_types> _Process_path_data(const path_builder<Allocator>& pf) {
					matrix_2d m;
					vector_2d origin;
					vector_2d currentPoint; // Tracks the untransformed current point.
					vector_2d closePoint;   // Tracks the transformed close point.
					::std::vector<path_data::path_data_types> v;

					for (const path_data::path_data_types& val : pf) {
						::std::visit([&m, &origin, &currentPoint, &closePoint, &v](auto&& item) {
							using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
							_Path_factory_process_visit<T>::template _Perform<T>(item, v, m, origin, currentPoint, closePoint);
						}, val);
					}
					return v;
				}

				template <class _TItem>
				struct _Path_factory_process_visit_noexcept {
					constexpr static double twoThirds = 2.0 / 3.0;

					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_new_path>, _Path_data_abs_new_path> = _Path_data_abs_new_path_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d&, vector_2d& currentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						currentPoint = item.at();
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_new_path>, pt);
						closePoint = pt;
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_new_path>, _Path_data_rel_new_path> = _Path_data_rel_new_path_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d&, optional<vector_2d>& currentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						currentPoint = currentPoint + item.at();
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_new_path>, pt);
						closePoint = pt;
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::close_path>, _Path_data_close_path> = _Path_data_close_path_val>
					static void _Perform(const T&, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint, ::std::error_code& ec) noexcept {
						v.emplace_back(::std::in_place_type<path_data::close_path>, closePoint);
						v.emplace_back(::std::in_place_type<path_data::abs_move>,
							closePoint);
						if (!m.is_finite() || !m.is_invertible()) {
							ec = ::std::make_error_code(io2d_error::invalid_matrix);
							return;
						}
						auto invM = matrix_2d{ m }.invert();
						// Need to assign the untransformed closePoint value to currentPoint.
						currentPoint = invM.transform_point(closePoint - origin) + origin;
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_matrix>, _Path_data_change_matrix> = _Path_data_change_matrix_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d& m, vector_2d&, vector_2d&, vector_2d&, ::std::error_code& ec) noexcept {
						if (!m.is_finite()) {
							ec = ::std::make_error_code(io2d_error::invalid_matrix);
							return;
						}
						if (!m.is_invertible()) {
							ec = ::std::make_error_code(io2d_error::invalid_matrix);
							return;
						}
						m = item.matrix();
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_origin>, _Path_data_change_origin> = _Path_data_change_origin_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d& origin, vector_2d&, vector_2d&, ::std::error_code& ec) noexcept {
						origin = item.origin();
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_cubic_curve>, _Path_data_abs_cubic_curve> = _Path_data_abs_cubic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint, ::std::error_code& ec) noexcept {
						auto pt1 = m.transform_point(item.control_point_1() - origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() - origin) + origin;
						auto pt3 = m.transform_point(item.end_point() - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, pt1,
							pt2, pt3);
						currentPoint = item.end_point();
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_line>, _Path_data_abs_line> = _Path_data_abs_line_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint, ::std::error_code& ec) noexcept {
						currentPoint = item.to();
						auto pt = m.transform_point(currentPoint.value() - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_quadratic_curve>, _Path_data_abs_quadratic_curve> = _Path_data_abs_quadratic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint, ::std::error_code& ec) noexcept {
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() - origin) + origin;
						auto endPt = m.transform_point(item.end_point() - origin) + origin;
						beginPt = m.transform_point(currentPoint.value() - origin) + origin;
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y() - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y() - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point();
						ec.clear();
					}
					//template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_arc_clockwise>, _Path_data_abs_arc_clockwise> = _Path_data_abs_arc_clockwise_val>
					//static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint, ::std::error_code& ec) noexcept {
					//	{
					//		auto ctr = item.center();
					//		auto endAng = item.end_angle();
					//		auto scl = item.scale();
					//		auto rad = (ctr - currentPoint).magnitude();
					//		//auto rad = item.radius();
					//		//auto ang1 = item.angle_1();
					//		//auto ang2 = item.angle_2();
					//		//while (ang2 < ang1) {
					//		//	ang2 += two_pi<double>;
					//		//}
					//		auto xDiff = currentPoint.x() - ctr.x();
					//		auto yDiff = currentPoint.y() - ctr.y();
					//		auto startAng = atan2(abs(yDiff), abs(xDiff));
					//		if (xDiff < 0.0) {
					//			if (yDiff >= 0.0) {
					//				startAng += pi<double>;
					//			}
					//			else {
					//				startAng += half_pi<double>;
					//			}
					//		}
					//		else {
					//			if (yDiff >= 0.0) {
					//				startAng += three_pi_over_two<double>;
					//			}
					//			// else leave it alone, it's already in the 0 to two pi range.
					//		}
					//		while ((endAng + two_pi<double>) < startAng) {
					//			endAng += two_pi<double>;
					//		}
					//		vector_2d pt0, pt1, pt2, pt3;
					//		int bezCount = 1;
					//		double theta = abs(endAng - startAng);
					//		//while (theta > two_pi<double>) {
					//		//	theta -= two_pi<double>;
					//		//}
					//		double phi{};
					//		while (theta >= half_pi<double>) {
					//			theta /= 2.0;
					//			bezCount += bezCount;
					//		}
					//		phi = theta / 2.0;
					//		auto cosPhi = cos(phi);
					//		auto sinPhi = sin(phi);
					//		pt0.x(cosPhi);
					//		pt0.y(-sinPhi);
					//		pt3.x(pt0.x());
					//		pt3.y(-pt0.y());
					//		pt1.x((4.0 - cosPhi) / 3.0);
					//		pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
					//		pt2.x(pt1.x());
					//		pt2.y(-pt1.y());
					//		phi = -phi;
					//		auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
					//			return { pt.x() * cos(a) + pt.y() * sin(a),
					//				-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
					//		};
					//		pt0 = rotCwFn(pt0, phi);
					//		pt1 = rotCwFn(pt1, phi);
					//		pt2 = rotCwFn(pt2, phi);
					//		pt3 = rotCwFn(pt3, phi);

					//		auto currTheta = startAng;
					//		//const auto startPt =
					//		//	ctr + rotCwFn({ pt0.x() * scl.x(), pt0.y() * scl.y() }, currTheta);

					//		//currentPoint = startPt;
					//		//auto pt = m.transform_point(currentPoint - origin) + origin;
					//		//v.emplace_back(::std::in_place_type<path_data::abs_new_path>, pt);
					//		//closePoint = pt;
					//		auto origOrigin = origin;
					//		origin = ctr - currentPoint;

					//		//FIXME ******** WE ARE CALCULATING UNIT ROTATIONS AROUND AN IDEAL CIRCULAR ARC - PREVIOUSLY RADIUS GAVE US THE TRANSFORM WE NEEDED BUT NOW WE NEED A SUBSTITUTE RADIUS
					//		//auto origM = m;
					//		//m = m.scale(scl);
					//		for (; bezCount > 0; bezCount--) {
					//			auto cpt1 = ctr + rotCwFn({ pt1.x() * rad, pt1.y() * rad }, currTheta);
					//			cpt1 *= scl;
					//			auto cpt2 = ctr + rotCwFn({ pt2.x() * rad, pt2.y() * rad },
					//				currTheta);
					//			cpt2 *= scl;
					//			auto cpt3 = ctr + rotCwFn({ pt3.x() * rad, pt3.y() * rad },
					//				currTheta);
					//			cpt3 *= scl;
					//			currentPoint = cpt3;
					//			cpt1 = m.transform_point(cpt1 - origin) + origin;
					//			cpt2 = m.transform_point(cpt2 - origin) + origin;
					//			cpt3 = m.transform_point(cpt3 - origin) + origin;
					//			v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1,
					//				cpt2, cpt3);
					//			currTheta += theta;
					//		}
					//		origin = origOrigin;
					//		ec.clear();
					//	}
					//}
					//template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_arc_clockwise>, _Path_data_rel_arc_clockwise> = _Path_data_rel_arc_clockwise_val>
					//static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint, error_code& ec) {
					//	path_data::abs_arc_clockwise absItem{};
					//	absItem.center(item.center() + currentPoint);
					//	absItem.end_angle(item.end_angle());
					//	absItem.scale(item.scale());
					//	_Path_factory_process_visit_noexcept::template _Perform(absItem, v, m, origin, currentPoint, closePoint, ec);
					//}
					//template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_arc_counterclockwise>, _Path_data_abs_arc_counterclockwise> = _Path_data_abs_arc_counterclockwise_val>
					//static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint, error_code& ec) {
					//	{
					//		auto ctr = item.center();
					//		auto endAng = item.end_angle();
					//		auto scl = item.scale();
					//		auto rad = (ctr - currentPoint).magnitude();
					//		//auto rad = item.radius();
					//		//auto ang1 = item.angle_1();
					//		//auto ang2 = item.angle_2();
					//		//while (ang2 > ang1) {
					//		//	ang2 -= two_pi<double>;
					//		//}
					//		auto xDiff = currentPoint.x() - ctr.x();
					//		auto yDiff = currentPoint.y() - ctr.y();
					//		auto startAng = atan2(abs(yDiff), abs(xDiff));
					//		if (xDiff < 0.0) {
					//			if (yDiff >= 0.0) {
					//				startAng += pi<double>;
					//			}
					//			else {
					//				startAng += half_pi<double>;
					//			}
					//		}
					//		else {
					//			if (yDiff >= 0.0) {
					//				startAng += three_pi_over_two<double>;
					//			}
					//			// else leave it alone, it's already in the 0 to two pi range.
					//		}
					//		while ((endAng + two_pi<double>) < startAng) {
					//			endAng += two_pi<double>;
					//		}
					//		vector_2d pt0, pt1, pt2, pt3;
					//		int bezCount = 1;
					//		double theta = abs(startAng - endAng);
					//		double phi{};
					//		while (theta >= half_pi<double>) {
					//			theta /= 2.0;
					//			bezCount += bezCount;
					//		}
					//		phi = theta / 2.0;
					//		auto cosPhi = cos(phi);
					//		auto sinPhi = sin(phi);
					//		pt0.x(cosPhi);
					//		pt0.y(-sinPhi);
					//		pt3.x(pt0.x());
					//		pt3.y(-pt0.y());
					//		pt1.x((4.0 - cosPhi) / 3.0);
					//		pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
					//		pt2.x(pt1.x());
					//		pt2.y(-pt1.y());
					//		auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
					//			return { pt.x() * cos(a) + pt.y() * sin(a),
					//				-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
					//		};
					//		pt0 = rotCwFn(pt0, phi);
					//		pt1 = rotCwFn(pt1, phi);
					//		pt2 = rotCwFn(pt2, phi);
					//		pt3 = rotCwFn(pt3, phi);
					//		auto shflPt = pt3;
					//		pt3 = pt0;
					//		pt0 = shflPt;
					//		shflPt = pt2;
					//		pt2 = pt1;
					//		pt1 = shflPt;
					//		auto currTheta = startAng;
					//		//const auto startPt = ctr + rotCwFn({ pt0.x() * rad, pt0.y() * rad }, currTheta);

					//		//currentPoint = startPt;
					//		//auto pt = m.transform_point(currentPoint - origin) + origin;
					//		//v.emplace_back(::std::in_place_type<path_data::abs_new_path>, pt);
					//		//closePoint = pt;
					//		auto origOrigin = origin;
					//		origin = ctr - currentPoint;

					//		for (; bezCount > 0; bezCount--) {
					//			auto cpt1 = ctr + rotCwFn({ pt1.x() * rad, pt1.y() * rad },
					//				currTheta);
					//			auto cpt2 = ctr + rotCwFn({ pt2.x() * rad, pt2.y() * rad },
					//				currTheta);
					//			auto cpt3 = ctr + rotCwFn({ pt3.x() * rad, pt3.y() * rad },
					//				currTheta);
					//			currentPoint = cpt3;
					//			cpt1 = m.transform_point(cpt1 - origin) + origin;
					//			cpt2 = m.transform_point(cpt2 - origin) + origin;
					//			cpt3 = m.transform_point(cpt3 - origin) + origin;
					//			v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1,
					//				cpt2, cpt3);
					//			currTheta -= theta;
					//		}
					//		origin = origOrigin;
					//		ec.clear();
					//	}
					//}
					//template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_arc_counterclockwise>, _Path_data_rel_arc_counterclockwise> = _Path_data_rel_arc_counterclockwise_val>
					//static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d& closePoint, error_code& ec) {
					//	path_data::abs_arc_counterclockwise absItem{};
					//	absItem.center(item.center() + currentPoint);
					//	absItem.end_angle(item.end_angle());
					//	absItem.scale(item.scale());
					//	_Path_factory_process_visit_noexcept::template _Perform(absItem, v, m, origin, currentPoint, closePoint, ec);
					//}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_cubic_curve>, _Path_data_rel_cubic_curve> = _Path_data_rel_cubic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						auto pt1 = m.transform_point(item.control_point_1() + currentPoint -
							origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() + currentPoint -
							origin) + origin;
						auto pt3 = m.transform_point(item.end_point() + currentPoint - origin) +
							origin;
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>,
							pt1, pt2, pt3);
						currentPoint = item.end_point() + currentPoint;
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_line>, _Path_data_rel_line> = _Path_data_rel_line_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						currentPoint = item.to() + currentPoint.value();
						auto pt = m.transform_point(currentPoint.value() - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
						ec.clear();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_quadratic_curve>, _Path_data_rel_quadratic_curve> = _Path_data_rel_quadratic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() + currentPoint - origin) + origin;
						auto endPt = m.transform_point(item.end_point() + currentPoint - origin) + origin;
						beginPt = m.transform_point(currentPoint - origin) + origin;
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y() - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y() - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point() + currentPoint;
						ec.clear();
					}
				};

				template <class Allocator>
				::std::vector<path_data::path_data_types> _Process_path_data(const path_builder<Allocator>& pf, ::std::error_code& ec) noexcept {
					matrix_2d m;
					vector_2d origin;
					vector_2d currentPoint; // Tracks the untransformed current point.
					bool hasCurrentPoint = false;
					vector_2d closePoint;   // Tracks the transformed close point.
					::std::vector<path_data::path_data_types> v;

					for (const path_data::path_data_types& val : pf) {
						::std::visit([&m, &origin, &currentPoint, &hasCurrentPoint, &closePoint, &v, &ec](auto&& item) {
							using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
							_Path_factory_process_visit_noexcept<T>::template _Perform<T>(item, v, m, origin, currentPoint, hasCurrentPoint, closePoint, ec);
						}, val);
						if (static_cast<bool>(ec)) {
							//v.clear();
							return v;
						}
					}
					return v;
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(const Allocator &a) noexcept
					: _Data(a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(size_type n, const Allocator & a)
					: _Data(n, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(size_type n, const value_type & value, const Allocator& a)
					: _Data(n, value, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				template<class InputIterator>
				inline path_builder<Allocator>::path_builder(InputIterator first, InputIterator last, const Allocator& a)
					: _Data(first, last, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				template<class InputIterator>
				inline void path_builder<Allocator>::assign(InputIterator first, InputIterator last) {
					_Data.assign(first, last);
				}

				template<class Allocator>
				template<class ...Args>
				inline typename path_builder<Allocator>::reference path_builder<Allocator>::emplace_back(Args && ...args) {
					return _Data.emplace_back(forward<Args>(args)...);
				}

				template<class Allocator>
				template<class ...Args>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::emplace(const_iterator position, Args&& ...args) {
					return _Data.emplace(position, forward<Args>(args)...);
				}

				template<class Allocator>
				template<class InputIterator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::insert(const_iterator position, InputIterator first, InputIterator last) {
					return _Data.template insert<InputIterator>(position, first, last);
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(const path_builder& pf)
					: _Data(pf._Data)
					, _Current_point(pf._Current_point)
					, _Last_move_to_point(pf._Last_move_to_point)
					, _Transform_matrix(pf._Transform_matrix)
					, _Origin(pf._Origin) {
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(path_builder&& pf) noexcept
					: _Data(move(pf._Data))
					, _Current_point(move(pf._Current_point))
					, _Last_move_to_point(move(pf._Last_move_to_point))
					, _Transform_matrix(move(pf._Transform_matrix))
					, _Origin(move(pf._Origin)) {
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(const path_builder& pf, const Allocator & a)
					: _Data(pf._Data, a)
					, _Current_point(pf._Current_point)
					, _Last_move_to_point(pf._Last_move_to_point)
					, _Transform_matrix(pf._Transform_matrix)
					, _Origin(pf._Origin) {
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(path_builder&& pf, const Allocator & a)
					: _Data(move(pf._Data), a)
					, _Current_point(move(pf._Current_point))
					, _Last_move_to_point(move(pf._Last_move_to_point))
					, _Transform_matrix(move(pf._Transform_matrix))
					, _Origin(move(pf._Origin)) {
				}

				template<class Allocator>
				inline path_builder<Allocator>::path_builder(initializer_list<value_type> il, const Allocator & a)
					: _Data(il, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template <class Allocator>
				inline path_builder<Allocator>& path_builder<Allocator>::operator=(const path_builder& x) {
					_Data = x._Data;
					_Current_point = x._Current_point;
					_Last_move_to_point = x._Last_move_to_point;
					_Origin = x._Origin;
					return *this;
				}
				template<class Allocator>
				inline path_builder<Allocator>& path_builder<Allocator>::operator=(path_builder&& x) noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value || allocator_traits<Allocator>::is_always_equal::value) {
					::std::swap(_Data, x._Data);
					::std::swap(_Current_point, x._Current_point);
					::std::swap(_Last_move_to_point, x._Last_move_to_point);
					::std::swap(_Origin, x._Origin);
					return *this;
				}
				template<class Allocator>
				inline path_builder<Allocator>& path_builder<Allocator>::operator=(initializer_list<value_type> il) {
					_Data.clear();
					for (const auto& item : il) {
						_Data.push_back(item);
					}
					return *this;
				}
				template<class Allocator>
				inline void path_builder<Allocator>::assign(size_type n, const value_type& u) {
					_Data.assign(n, u);
				}
				template<class Allocator>
				inline void path_builder<Allocator>::assign(initializer_list<value_type> il) {
					_Data.assign(il);
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::allocator_type path_builder<Allocator>::get_allocator() const noexcept {
					return _Data.allocator_type();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::begin() noexcept {
					return _Data.begin();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_iterator path_builder<Allocator>::begin() const noexcept {
					return _Data.begin();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_iterator path_builder<Allocator>::cbegin() const noexcept {
					return _Data.cbegin();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::end() noexcept {
					return _Data.end();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_iterator path_builder<Allocator>::end() const noexcept {
					return _Data.end();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_iterator path_builder<Allocator>::cend() const noexcept {
					return _Data.cend();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::reverse_iterator path_builder<Allocator>::rbegin() noexcept {
					return _Data.rbegin();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_reverse_iterator path_builder<Allocator>::rbegin() const noexcept {
					return _Data.rbegin();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_reverse_iterator path_builder<Allocator>::crbegin() const noexcept {
					return _Data.crbegin();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::reverse_iterator path_builder<Allocator>::rend() noexcept {
					return _Data.rend();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_reverse_iterator path_builder<Allocator>::rend() const noexcept {
					return _Data.rend();
				}
				template<class Allocator>
				inline typename path_builder<Allocator>::const_reverse_iterator path_builder<Allocator>::crend() const noexcept {
					return _Data.crend();
				}

				template<class Allocator>
				inline bool path_builder<Allocator>::empty() const noexcept {
					return _Data.empty();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::size_type path_builder<Allocator>::size() const noexcept {
					return _Data.size();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::size_type path_builder<Allocator>::max_size() const noexcept {
					return _Data.max_size();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::size_type path_builder<Allocator>::capacity() const noexcept {
					return _Data.capacity();
				}

				template<class Allocator>
				inline void path_builder<Allocator>::resize(size_type sz) {
					_Data.resize(sz);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::resize(size_type sz, const value_type& c) {
					_Data.resize(sz, c);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::reserve(size_type n) {
					_Data.reserve(n);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::shrink_to_fit() {
					_Data.shrink_to_fit();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::reference path_builder<Allocator>::operator[](size_type n) {
					return _Data[n];
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::const_reference path_builder<Allocator>::operator[](size_type n) const {
					return _Data[n];
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::const_reference path_builder<Allocator>::at(size_type n) const {
					return _Data.at(n);
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::reference path_builder<Allocator>::at(size_type n) {
					return _Data.at(n);
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::reference path_builder<Allocator>::front() {
					return _Data.front();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::const_reference path_builder<Allocator>::front() const {
					return _Data.front();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::reference path_builder<Allocator>::back() {
					return _Data.back();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::const_reference path_builder<Allocator>::back() const {
					return _Data.back();
				}

				template<class Allocator>
				inline void path_builder<Allocator>::new_path(const vector_2d& v) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_new_path>, v);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::rel_new_path(const vector_2d& v) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_new_path>, v);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::close_path() noexcept {
					const vector_2d pt{};
					_Data.emplace_back(in_place_type<path_data::close_path>, pt);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::arc(const vector_2d& rad, double rot, const double sang) noexcept {
					_Data.emplace_back(in_place_type<path_data::arc>, rad, rot, sang);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::cubic_curve_to(const vector_2d& pt0, const vector_2d& pt1, const vector_2d& pt2) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_cubic_curve>, pt0, pt1, pt2);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::line_to(const vector_2d& pt) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_line>, pt);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::quadratic_curve_to(const vector_2d& pt0, const vector_2d& pt1) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_quadratic_curve>, pt0, pt1);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::rel_cubic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1, const vector_2d& dpt2) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_cubic_curve>, dpt0, dpt1, dpt2);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::rel_line_to(const vector_2d& dpt) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_line>, dpt);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::rel_quadratic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_quadratic_curve>, dpt0, dpt1);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::transform_matrix(const matrix_2d& m) noexcept {
					_Transform_matrix = m;
					_Data.emplace_back(in_place_type<path_data::change_matrix>, m);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::origin(const vector_2d& pt) noexcept {
					_Origin = pt;
					_Data.emplace_back(in_place_type<path_data::change_origin>, pt);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::push_back(const value_type& x) {
					_Data.push_back(x);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::push_back(value_type&& x) {
					_Data.push_back(move(x));
				}

				template<class Allocator>
				inline void path_builder<Allocator>::pop_back() {
					_Data.pop_back();
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::insert(const_iterator position, const value_type& x) {
					return _Data.insert(position, x);
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::insert(const_iterator position, value_type&& x) {
					return _Data.insert(position, x);
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::insert(const_iterator position, size_type n, const value_type& x) {
					return _Data.insert(position, n, x);
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::insert(const_iterator position, initializer_list<value_type> il) {
					return _Data.insert(position, il);
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::erase(const_iterator position) {
					return _Data.erase(position);
				}

				template<class Allocator>
				inline typename path_builder<Allocator>::iterator path_builder<Allocator>::erase(const_iterator first, const_iterator last) {
					return _Data.erase(first, last);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::swap(path_builder &pf) noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value || allocator_traits<Allocator>::is_always_equal::value) {
					swap(_Data, pf._Data);
					swap(_Current_point, pf._Current_point);
					swap(_Last_move_to_point, pf._Last_move_to_point);
					swap(_Origin, pf._Origin);
					swap(_Transform_matrix, pf._Transform_matrix);
				}

				template<class Allocator>
				inline void path_builder<Allocator>::clear() noexcept {
					_Data.clear();
					_Current_point.reset();
					_Transform_matrix = matrix_2d::init_identity();
					_Origin = {};
				}

				template <class InputIterator>
				inline brush::brush(const vector_2d& begin, const vector_2d& end,
					InputIterator first, InputIterator last)
					: _Brush()
					, _Image_surface()
					, _Brush_type(brush_type::linear) {
					_Brush = shared_ptr<cairo_pattern_t>(cairo_pattern_create_linear(begin.x(), begin.y(), end.x(), end.y()), &cairo_pattern_destroy);
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));

					for (auto it = first; it != last; ++it) {
						auto stop = *it;
						cairo_pattern_add_color_stop_rgba(_Brush.get(), stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
					}
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));
				}

				template <class InputIterator>
				inline brush::brush(const circle& start, const circle& end,
					InputIterator first, InputIterator last)
					: _Brush()
					, _Brush_type(brush_type::radial) {
					_Brush = shared_ptr<cairo_pattern_t>(cairo_pattern_create_radial(start.center().x(), start.center().y(), start.radius(), end.center().x(), end.center().y(), end.radius()), &cairo_pattern_destroy);
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));
					for (auto it = first; it != last; ++it) {
						auto stop = *it;
						cairo_pattern_add_color_stop_rgba(_Brush.get(), stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
					}
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));
				}

#if defined(_MSC_VER) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif
				// Creates the appropriate error_code for a given cairo_status_t value.
				// cairo_status_t values which are implementation detail errors are all mapped to make_error_code(io2d_error::invalid_status).
				inline ::std::error_code _Cairo_status_t_to_std_error_code(cairo_status_t cs) noexcept {
					switch (cs) {
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 13, 1)
					case CAIRO_STATUS_JBIG2_GLOBAL_MISSING:
						assert(false && "Unexpected post cairo 1.12.16 value CAIRO_STATUS_JBIG2_GLOBAL_MISSING, though this should not occur due to cairo's compatibility guarantee.");
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
#endif
					case CAIRO_STATUS_SUCCESS:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::success);
					case CAIRO_STATUS_NO_MEMORY:
						return ::std::make_error_code(::std::errc::not_enough_memory);
					case CAIRO_STATUS_INVALID_RESTORE:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_restore);
					case CAIRO_STATUS_INVALID_POP_GROUP:
						assert(false && "Unexpected value CAIRO_STATUS_INVALID_POP_GROUP.");
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_NO_CURRENT_POINT:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_path_data);
					case CAIRO_STATUS_INVALID_MATRIX:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_matrix);
					case CAIRO_STATUS_INVALID_STATUS:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_NULL_POINTER:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::null_pointer);
					case CAIRO_STATUS_INVALID_STRING:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_string);
					case CAIRO_STATUS_INVALID_PATH_DATA:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_path_data);
					case CAIRO_STATUS_READ_ERROR:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::read_error);
					case CAIRO_STATUS_WRITE_ERROR:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::write_error);
					case CAIRO_STATUS_SURFACE_FINISHED:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::surface_finished);
					case CAIRO_STATUS_SURFACE_TYPE_MISMATCH:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_PATTERN_TYPE_MISMATCH:
						assert(false && "Unexpected value CAIRO_STATUS_PATTERN_TYPE_MISMATCH.");
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_INVALID_CONTENT:
						return ::std::make_error_code(::std::errc::invalid_argument);
					case CAIRO_STATUS_INVALID_FORMAT:
						return ::std::make_error_code(::std::errc::invalid_argument);
					case CAIRO_STATUS_INVALID_VISUAL:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_FILE_NOT_FOUND:
						return ::std::make_error_code(::std::errc::no_such_file_or_directory);
					case CAIRO_STATUS_INVALID_DASH:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_dash);
					case CAIRO_STATUS_INVALID_DSC_COMMENT:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_INVALID_INDEX:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_index);
					case CAIRO_STATUS_CLIP_NOT_REPRESENTABLE:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::clip_not_representable);
					case CAIRO_STATUS_TEMP_FILE_ERROR:
						// Even though it's an I/O error, this is an implementation detail error and as such is invalid_status.
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_INVALID_STRIDE:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_stride);
					case CAIRO_STATUS_FONT_TYPE_MISMATCH:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_USER_FONT_IMMUTABLE:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::user_font_immutable);
					case CAIRO_STATUS_USER_FONT_ERROR:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::user_font_error);
					case CAIRO_STATUS_NEGATIVE_COUNT:
						assert(false && "CAIRO_STATUS_NEGATIVE_COUNT should not occur because the library API should prevent the existence of negative count values.");
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_INVALID_CLUSTERS:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_clusters);
					case CAIRO_STATUS_INVALID_SLANT:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_INVALID_WEIGHT:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_INVALID_SIZE:
						return ::std::make_error_code(::std::errc::invalid_argument);
					case CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED:
						assert(false && "CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED encountered but the API doesn't expose user fonts so this status should never occur. What happened?");
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_DEVICE_TYPE_MISMATCH:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_DEVICE_ERROR:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::device_error);
					case CAIRO_STATUS_INVALID_MESH_CONSTRUCTION:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_mesh_construction);
					case CAIRO_STATUS_DEVICE_FINISHED:
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					case CAIRO_STATUS_LAST_STATUS:
						assert(false && "Unexpected value CAIRO_STATUS_LAST_STATUS. The runtime version of cairo is likely newer than the version of cairo this implementation was compiled against.");
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					default:
						assert(false && "Unknown cairo_status_t value caught by default case. The runtime version of cairo is likely newer than the version of cairo this implementation was compiled against, though this still should not occur due to cairo's compatibility guarantee.");
						return ::std::make_error_code(::std::experimental::io2d::io2d_error::invalid_status);
					}
				}
#if defined(_MSC_VER) && defined(__clang__)
#pragma clang diagnostic pop
#endif

				// Throws an exception of type:
				// -  bad_alloc : If the value of s is CAIRO_STATUS_NO_MEMORY.
				// -  invalid_argument: If the value of s is CAIRO_STATUS_INVALID_FORMAT, CAIRO_STATUS_INVALID_CONTENT, or CAIRO_STATUS_INVALID_SIZE.
				// -  out_of_range : If the value of s is CAIRO_STATUS_INVALID_INDEX.
				// -  system_error : For all other s values, with its error_code being the return value of _Cairo_status_t_to_std_error_code(s).
				// If the value of s is CAIRO_STATUS_LAST_STATUS, CAIRO_STATUS_INVALID_POP_GROUP, CAIRO_STATUS_NEGATIVE_COUNT, CAIRO_STATUS_PATTERN_TYPE_MISMATCH,
				// CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED, or an unknown value, it triggers an assertion failure. If assertion checks are disabled, an exception
				// of type system_error with an error code of make_error_code(io2d_error::invalid_status) will be thrown.
				inline void _Throw_if_failed_cairo_status_t(::cairo_status_t s) {
					assert(s != CAIRO_STATUS_LAST_STATUS && s != CAIRO_STATUS_INVALID_POP_GROUP && s != CAIRO_STATUS_NEGATIVE_COUNT);
					if (s != CAIRO_STATUS_SUCCESS) {
						if (s == CAIRO_STATUS_NO_MEMORY) {
							throw ::std::bad_alloc{};
						}
						else {
							if (s == CAIRO_STATUS_INVALID_FORMAT || s == CAIRO_STATUS_INVALID_CONTENT || s == CAIRO_STATUS_INVALID_SIZE) {
								if (s == CAIRO_STATUS_INVALID_FORMAT) {
									throw ::std::invalid_argument{ "The value of a format type argument is invalid." };
								}
								else {
									if (s == CAIRO_STATUS_INVALID_CONTENT) {
										throw ::std::invalid_argument{ "The value of a content type argument is invalid." };
									}
									else {
										throw ::std::invalid_argument{ "A value of a size parameter is invalid." };
									}
								}
							}
							else {
								if (s == CAIRO_STATUS_INVALID_INDEX) {
									throw ::std::out_of_range{ "An index parameter has an invalid value." };
								}
							}

							throw ::std::system_error(_Cairo_status_t_to_std_error_code(s));
						}
					}
				}

				// Checks for equality between two floating point numbers using an epsilon value to specify the equality tolerance limit.
				// See: http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
				template <typename T>
				inline bool _Almost_equal_relative(T a, T b, T epsilon = ::std::numeric_limits<T>::epsilon()) noexcept {
					auto diff = ::std::abs(a - b);
					a = ::std::abs(a);
					b = ::std::abs(b);
					auto largest = (b > a) ? b : a;
					if (diff <= largest * epsilon) {
						return true;
					}
					return false;
				}

				// Returns the result of adding 'center' to the result of rotating the point { 'radius', 0.0 } 'angle' radians around { 0.0, 0.0 } in a clockwise ('clockwise' == true) or
				// counterclockwise ('clockwise' == false) direction.
				inline ::std::experimental::io2d::vector_2d _Rotate_point_absolute_angle(const ::std::experimental::io2d::vector_2d& center, double radius, double angle, bool clockwise = true) {
					if (clockwise) {
						::std::experimental::io2d::vector_2d pt{ radius * ::std::cos(angle), -(radius * -::std::sin(angle)) };
						pt.x(pt.x() + center.x());
						pt.y(pt.y() + center.y());
						return pt;
					}
					else {
						::std::experimental::io2d::vector_2d pt{ radius * ::std::cos(angle), radius * -::std::sin(angle) };
						pt.x(pt.x() + center.x());
						pt.y(pt.y() + center.y());
						return pt;
					}
				}

				// Converts 'value' to an int and returns it. If nearestNeighbor is true, the return value is the result of calling 'static_cast<int>(round(value))'; if false, the return value is the result of calling 'static_cast<int>(trunc(value))'.
				inline int _Double_to_int(double value, bool nearestNeighbor = true) {
					if (nearestNeighbor) {
						// Round to the nearest neighbor.
						return static_cast<int>(::std::round(value));
					}
					// Otherwise truncate.
					return static_cast<int>(::std::trunc(value));
				}

				template <typename T>
				inline int _Container_size_to_int(const T& container) noexcept {
					assert(container.size() <= static_cast<unsigned int>(::std::numeric_limits<int>::max()));
					return static_cast<int>(container.size());
				}
			}
		}
	}
}

#endif
