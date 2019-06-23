#pragma once

class XrPPDStream
{
private:
	bsize m_buffer_size;
	u8* m_buffer;
	u8* m_pointer;

public:
	IC XrPPDStream(const void* buffer, const bsize& buffer_size)
	{
		m_buffer_size = buffer_size;
		m_buffer = (u8*)buffer;
		m_pointer = m_buffer;
	}
	IC void put_char(const u8& object)
	{
		VERIFY(m_pointer >= m_buffer);
		VERIFY(m_pointer < (m_buffer + m_buffer_size));
		*m_pointer = object;
		++m_pointer;
	}
	IC int get_char()
	{
		VERIFY(m_pointer >= m_buffer);
		if (m_pointer < (m_buffer + m_buffer_size))
			return (*m_pointer++);
		return (EOF);
	}
	IC void rewind()
	{
		m_pointer = m_buffer;
	}
	IC u8* buffer() const
	{
		return (m_buffer);
	}

	IC bsize tell() const
	{
		VERIFY(m_pointer >= m_buffer);
		VERIFY2(m_pointer <= (m_buffer + m_buffer_size), make_string("0x%08x <= 0x%08x + %d", m_pointer, m_buffer, m_buffer_size));
		return (u32(m_pointer - m_buffer));
	}
};