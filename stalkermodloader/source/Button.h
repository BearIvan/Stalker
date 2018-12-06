namespace Modloader
{
	class Button
	{
		friend class Window;
	protected:
		Button(
#ifdef WINDOWS
			HWND
#elif LINUX
			void*
#endif
			Handle,
			const bchar*text,
			bsize x,bsize y,
			bsize width,bsize hieght
		);

	public:
		~Button();
#ifdef WINDOWS
		HWND
#elif LINUX
		void*
#endif
			m_handle;
	};
}