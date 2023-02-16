#pragma once
#include "pch.h"
#include "xre/Macros.h"


namespace XRE {
	//�¼�����
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//λ����ʵ��mask1|mask2 ��������
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};
	//�궨�壺��ȡ�¼�������صķ���
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
	//�¼�����
	class Event
	{
		friend class EventDispatcher;
	public:
		//�Ƿ��Ѿ�����
		bool Handled = false;

		//���������Ϸ��궨�帲д
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		//ͨ��mask��λ�����ж�����
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		

	};

	class EventDispatcher
	{
		//�ô���ζ�����һ�����ͱ��� EventFn������ʾһ�ֿ���������T���ʹ�õĺ����������ͣ�
		//�ú������������һ������ֵ����Ϊ bool�����ҽ���һ������T�����ò����Ŀɵ��ö���
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}
		//����һ���������ͺ�������dispatcher�м�¼���¼���Ϊ�������ݸ��ú��������ã������¼��Ĵ���״̬
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	//�����������������¼�������
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
