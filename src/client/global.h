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

		std::function<void()> _reset;
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
		if constexpr (is_enum_v<T>)
		{
			return format("({}:{})", to_underlying(v), enum_name(v));
		}
		else if constexpr (is_same_v<bool, T>)
		{
			using enum FormatType;
			switch (type)
			{
			case bool_Switch:			return format("{}", v ? "On" : "Off");
			case bool_State:			return format("{}", v ? "True" : "False");
			case bool_Status:			return format("{}", v ? "Enabled" : "Disabled");
			case bool_Result:			return format("{}", v ? "Success" : "Failure");
			case bool_Affirmation:	return format("{}", v ? "Yes" : "No");
			default:						return format("{}", v);
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

	using FontNames = const vector<string>;
	extern FontNames kNames;
}

using SceneID = Global::scene::ID;