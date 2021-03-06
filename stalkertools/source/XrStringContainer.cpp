#include "stdafx.h"
constexpr bsize  HEADER = sizeof(XrStringContainerValue);
namespace Impl
{
	struct XrStringContainer
	{


		static const u32 buffer_size = 1024 * 256;
		XrStringContainerValue* buffer[buffer_size];
		int num_docs;

		XrStringContainer()
		{
			num_docs = 0;
			ZeroMemory(buffer, sizeof(buffer));
		}

		XrStringContainerValue* find(XrStringContainerValue* value, const char* str)
		{
			XrStringContainerValue* candidate = buffer[value->dwCRC % buffer_size];
			while (candidate)
			{
				if (candidate->dwCRC == value->dwCRC &&
					candidate->dwLength == value->dwLength &&
					!memcmp(candidate->value, str, value->dwLength))
				{
					return candidate;
				}

				candidate = candidate->next;
			}

			return NULL;
		}

		void insert(XrStringContainerValue* value)
		{
			XrStringContainerValue** element = &buffer[value->dwCRC % buffer_size];
			value->next = *element;
			*element = value;
		}

		void clean()
		{
			for (u32 i = 0; i < buffer_size; ++i)
			{
				XrStringContainerValue** current = &buffer[i];

				while (*current != NULL)
				{
					XrStringContainerValue* value = *current;
					if (!value->dwReference)
					{
						*current = value->next;
						bear_free(value);
					}
					else
					{
						current = &value->next;
					}
				}
			}
		}

		void verify()
		{
			Msg("strings verify started");
			for (u32 i = 0; i < buffer_size; ++i)
			{
				XrStringContainerValue* value = buffer[i];
				while (value)
				{
					u32 crc = BearCheckSum::CRC32(value->value, value->dwLength);
					//string32 crc_str;
					BEAR_RASSERT(crc == value->dwCRC);
					BEAR_RASSERT(value->dwLength == BearString::GetSize(value->value));
					value = value->next;
				}
			}
			Msg("strings verify completed");
		}

		void dump(FILE* f) const
		{
			/*for (u32 i = 0; i < buffer_size; ++i)
			{
				XrStringContainerValue* value = buffer[i];
				while (value)
				{
					fprintf(f, "ref[%4u]-len[%3u]-crc[%8X] : %s\n", value->dwReference, value->dwLength, value->dwCRC, value->value);
					value = value->next;
				}
			}*/
		}

		void dump(IWriter* f) const
		{
			/*for (u32 i = 0; i < buffer_size; ++i)
			{
				XrStringContainerValue* value = buffer[i];
				string4096 temp;
				while (value)
				{
					xr_sprintf(temp, sizeof(temp), "ref[%4u]-len[%3u]-crc[%8X] : %s\n", value->dwReference, value->dwLength, value->dwCRC, value->value);
					f->w_string(temp);
					value = value->next;
				}
			}*/
		}

		bsize stat_economy()
		{
			bsize counter = 0;
			for (u32 i = 0; i < buffer_size; ++i)
			{
				XrStringContainerValue* value = buffer[i];
				while (value)
				{
					counter -= sizeof(XrStringContainerValue);
					counter += (value->dwReference - 1)*(value->dwLength + 1);
					value = value->next;
				}
			}

			return counter;
		}
	};
}


xrCriticalSection *XrStringContainer::cs=0;
Impl::XrStringContainer* XrStringContainer::impl=0;
void XrStringContainer::Initialize()
{
	impl = bear_new<Impl::XrStringContainer>();
	cs = bear_new<xrCriticalSection>();
}

void XrStringContainer::Destroy()
{
	clean();
	bear_delete(impl);
	bear_delete(cs);
}

XrStringContainerValue* XrStringContainer::dock(const bchar* value)
{
	if (0 == value) return 0;

	cs->Enter();

#ifdef DEBUG_MEMORY_MANAGER
	Memory.stat_strdock++;
#endif // DEBUG_MEMORY_MANAGER

	XrStringContainerValue* result = 0;

	// calc len
	bsize s_len = BearString::GetSize(value);
	bsize s_len_with_zero = (bsize)s_len + 1;
	VERIFY(HEADER + s_len_with_zero < 4096);

	// setup find structure
	char header[sizeof(XrStringContainerValue)];
	XrStringContainerValue* sv = (XrStringContainerValue*)header;
	sv->dwReference = 0;
	sv->dwLength = s_len;
	sv->dwCRC = BearCheckSum::CRC32(value, s_len);

	// search
	result = impl->find(sv, value);

#ifdef DEBUG
	bool is_leaked_string = !xr_strcmp(value, "enter leaked string here");
#endif //DEBUG

	// it may be the case, string is not found or has "non-exact" match
	if (0 == result
#ifdef DEBUG
		|| is_leaked_string
#endif //DEBUG
		)
	{

		result = (XrStringContainerValue*)BearMemory::Malloc(HEADER + s_len_with_zero
			, "storage: sstring"
		);

#ifdef DEBUG
		static int num_leaked_string = 0;
		if (is_leaked_string)
		{
			++num_leaked_string;
			Msg("leaked_string: %d 0x%08x", num_leaked_string, result);
		}
#endif // DEBUG

		result->dwReference = 0;
		result->dwLength = sv->dwLength;
		result->dwCRC = sv->dwCRC;
		result->next = 0;
		CopyMemory((uint8*)result+ HEADER, value, s_len_with_zero);
		result->value = (bchar8*)result + HEADER;

		impl->insert(result);
	}
	cs->Leave();

	return result;
}

void XrStringContainer::clean()
{
	cs->Enter();
	impl->clean();
	cs->Leave();
}

void XrStringContainer::verify()
{
	cs->Enter();
	impl->verify();
	cs->Leave();
}

void XrStringContainer::dump()
{
	return;/*
	cs->Enter();
	FILE* F = fopen("d:\\$str_dump$.txt", "w");
	impl->dump(F);
	fclose(F);
	cs->Leave();*/
}

void XrStringContainer::dump(IWriter* W)
{
	cs->Enter();
	impl->dump(W); 
	cs->Leave();
}

bsize XrStringContainer::stat_economy()
{
	cs->Enter();
	bsize counter = 0;
	counter -= sizeof(XrStringContainer);
	counter += impl->stat_economy();
	cs->Leave();
	return u32(counter);
}

