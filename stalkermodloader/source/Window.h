namespace Modloader
{
	class Window
	{
	public:
		Window(bsize wigth, bsize height);
		inline
#ifdef WINDOWS
			HWND
#elif LINUX
			void*
#endif
			GetWindowHandle() const
		{
			return m_window;
		}
		bool Update();
		~Window();
		void OnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	protected:
		Window& CreateButton( const bchar*text, bsize x, bsize y, bsize width, bsize height);
		Window& CreateCombobox(const bchar*name, const bchar*text, bsize x, bsize y, bsize width, bsize height);
	private:
		BearCore::BearMap<  bsize, BearCore::BearMemoryRef<Button> > m_buttons_map;
		BearCore::BearMap< BearCore::BearStringConteniar, BearCore::BearMemoryRef<Combobox> > m_comboboxs_map;
#ifdef WINDOWS
		HWND
#elif LINUX
		void*
#endif
			m_window;
	};
};