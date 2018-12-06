namespace Modloader
{
	class Combobox
	{
		friend class Window;
	protected:
		Combobox(
#ifdef WINDOWS
			HWND
#elif LINUX
			void*
#endif
			Handle,
			const bchar*text,
			bsize x, bsize y,
			bsize width, bsize hieght
		);

	public:
		~Combobox();
#ifdef WINDOWS
		HWND
#elif LINUX
		void*
#endif
			m_handle;
	};
}