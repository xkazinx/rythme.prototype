#pragma once

class App;
class SceneMgr;
class CommandMgr;

namespace Global
{
	constexpr char kAppName[] = "Rythme";

	extern App* g_app;
	extern unique_ptr<SceneMgr> g_scene;
	extern unique_ptr<CommandMgr> g_cmd;
	extern u16 g_res_x, g_res_y;
	static constexpr u8 kCommandSz = 112 + 1; // 1 for imgui's '\0' ending, maybe

	using ButtonIdx = u8;
	static constexpr ButtonIdx kButtonMax = 10; // 1 for imgui's '\0' ending, maybe


	extern void Initialize();
	extern void Deinitialize();

	// Console lines (the old kazin library's Print / PrintDebug / PrintError): std::format, coloured with rang
	template<typename... Args>
	void Print(const std::format_string<Args...> fmt, Args &&... args)
	{
		std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
	}

	template<typename... Args>
	void PrintDebug(const std::format_string<Args...> fmt, Args &&... args)
	{
		std::cout << rang::fg::gray << std::format(fmt, std::forward<Args>(args)...) << rang::style::reset << '\n';
	}

	template<typename... Args>
	void PrintError(const std::format_string<Args...> fmt, Args &&... args)
	{
		std::cout << rang::fg::red << std::format(fmt, std::forward<Args>(args)...) << rang::style::reset << '\n';
	}
	
	class Guard
	{
	public:
		template<typename T>
		Guard(T func)
			: _reset(func) {

		}
		~Guard() {
			_reset();
		}

		function<void()> _reset;
	};

	namespace scene
	{
		enum ID : u8
		{
			Main,
			Game,
		};
	}
	
	enum class FormatType : u8
	{
		None,
		bool_Switch,		// On, Off
		bool_State,			// true, false
		bool_Status,		// Enabled, Disabled
		bool_Result,		// Success, Failure
		bool_Affirmation, // Yes, No
	};

	template<typename T>
	inline string Format(T v, FormatType type = FormatType::None)
	{
		if constexpr (std::is_enum_v<T>)
		{
			return std::format("({}:{})", Under(v), magic_enum::enum_name(v));
		}
		else if constexpr (is_same_v<bool, T>)
		{
			using enum FormatType;
			switch (type)
			{
			case bool_Switch:			return v ? "On" : "Off";
			case bool_State:			return v ? "True" : "False";
			case bool_Status:			return v ? "Enabled" : "Disabled";
			case bool_Result:			return v ? "Success" : "Failure";
			case bool_Affirmation:	return v ? "Yes" : "No";
			default:						return std::format("{}", v);
			}
		}
		else
		{
			return to_string(v);
		}
	}
}

namespace font {
	enum class ID : u8 {
		Arial,
		Max,
	};

	using FontNames = const array<string_view, 1>;
	extern FontNames kNames;
}

using SceneID = Global::scene::ID;