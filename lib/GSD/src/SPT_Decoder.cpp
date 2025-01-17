#include "SPT_Decoder.h"

#include <cmath>


namespace GSD::SPT
{
	static uint32_t sg_aTable[128] =
	{
		0x00000005, 0x00000001, 0xFFFFFFFE, 0x00000000, 0x00000002, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFA,
		0x00000006, 0x00000003, 0x00000000, 0xFFFFFFFD, 0x00000003, 0xFFFFFFFC, 0xFFFFFFFD, 0xFFFFFFFE,
		0x00000007, 0x00000005, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF9,
		0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD,
		0x00000002, 0x00000003, 0x00000005, 0x00000002, 0xFFFFFFFD, 0x00000001, 0xFFFFFFFA, 0xFFFFFFFC,
		0x00000004, 0x00000004, 0x00000001, 0x00000004, 0x00000002, 0xFFFFFFFB, 0xFFFFFFFC, 0xFFFFFFFA,
		0x00000001, 0x00000002, 0x00000002, 0x00000003, 0x00000001, 0x00000002, 0xFFFFFFFC, 0xFFFFFFF9,
		0x00000002, 0xFFFFFFFF, 0x00000001, 0x00000004, 0x00000002, 0xFFFFFFFF, 0xFFFFFFFB, 0xFFFFFFFE,
		0x00000002, 0x00000006, 0xFFFFFFFF, 0x00000000, 0x00000001, 0xFFFFFFFB, 0xFFFFFFFE, 0xFFFFFFFF,
		0x00000003, 0x00000004, 0x00000000, 0x00000003, 0xFFFFFFFD, 0x00000002, 0xFFFFFFFA, 0xFFFFFFFD,
		0x00000007, 0x00000005, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF9,
		0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD,
		0x00000006, 0x00000003, 0xFFFFFFFE, 0x00000004, 0xFFFFFFFD, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFB,
		0x00000005, 0x00000006, 0x00000004, 0xFFFFFFFF, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFE, 0xFFFFFFFC,
		0x00000007, 0xFFFFFFFF, 0x00000004, 0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFD, 0xFFFFFFFE,
		0x00000001, 0x00000005, 0xFFFFFFFE, 0xFFFFFFFF, 0x00000001, 0x00000002, 0xFFFFFFFE, 0xFFFFFFFC
	};


	void Coder::DecodeRound0(uint8_t* pData, size_t nSize, size_t nType)
	{
		switch (nType)
		{
		case 0:
		{
			size_t dec_size = nSize - 2;
			for (size_t ite_bytes = 0; ite_bytes <= dec_size; ite_bytes += 2)
			{
				uint8_t tmp_byte_0 = pData[ite_bytes + 0];
				uint8_t tmp_byte_1 = pData[ite_bytes + 1];
				pData[ite_bytes + 0] = tmp_byte_1;
				pData[ite_bytes + 1] = tmp_byte_0;
			}
		}
		break;

		case 1:
		{
			size_t dec_size = nSize - 4;
			for (size_t ite_bytes = 0; ite_bytes <= dec_size; ite_bytes += 4)
			{
				uint8_t tmp_byte_0 = pData[ite_bytes + 0];
				uint8_t tmp_byte_1 = pData[ite_bytes + 1];
				uint8_t tmp_byte_2 = pData[ite_bytes + 2];
				uint8_t tmp_byte_3 = pData[ite_bytes + 3];
				pData[ite_bytes + 0] = tmp_byte_2;
				pData[ite_bytes + 1] = tmp_byte_3;
				pData[ite_bytes + 2] = tmp_byte_0;
				pData[ite_bytes + 3] = tmp_byte_1;
			}
		}
		break;

		case 2:
		{
			size_t dec_size = nSize - 8;
			for (size_t ite_bytes = 0; ite_bytes <= dec_size; ite_bytes += 8)
			{
				uint8_t tmp_byte_0 = pData[ite_bytes + 0];
				uint8_t tmp_byte_1 = pData[ite_bytes + 1];
				uint8_t tmp_byte_2 = pData[ite_bytes + 2];
				uint8_t tmp_byte_3 = pData[ite_bytes + 3];
				uint8_t tmp_byte_4 = pData[ite_bytes + 4];
				uint8_t tmp_byte_5 = pData[ite_bytes + 5];
				uint8_t tmp_byte_6 = pData[ite_bytes + 6];
				uint8_t tmp_byte_7 = pData[ite_bytes + 7];
				pData[ite_bytes + 0] = tmp_byte_6;
				pData[ite_bytes + 1] = tmp_byte_4;
				pData[ite_bytes + 2] = tmp_byte_5;
				pData[ite_bytes + 3] = tmp_byte_7;
				pData[ite_bytes + 4] = tmp_byte_1;
				pData[ite_bytes + 5] = tmp_byte_2;
				pData[ite_bytes + 6] = tmp_byte_0;
				pData[ite_bytes + 7] = tmp_byte_3;
			}
		}
		break;
		}
	}

	void Coder::DecodeRound1(uint32_t* pTable, uint8_t* pData, size_t nSize, size_t nStart)
	{
		if (nStart >= 8) { return; }

		for (size_t ite_bytes = 0; ite_bytes < nSize; ++ite_bytes)
		{
			uint8_t tmp_byte = pData[ite_bytes];
			uint8_t decode_byte = 0;
			for (size_t ite_table = 0; ite_table < 8; ++ite_table)
			{
				uint32_t shift_value = pTable[8 * nStart + 64 + ite_table];
				if ((int)shift_value <= -1)
				{
					decode_byte |= ((1 << ite_table) & tmp_byte) >> std::abs((int)shift_value);
				}
				else
				{
					decode_byte |= ((1 << ite_table) & tmp_byte) << shift_value;
				}
			}
			pData[ite_bytes] = ~decode_byte;
		}
	}


	void Coder::Decode(uint8_t* pData, size_t nSize, bool isMakeReadable)
	{
		size_t start_index = pData[0] ^ 0xF0;
		size_t decode_type = pData[1] ^ 0xF0;
		uint8_t* enc_data_ptr = pData + 0x4;
		size_t enc_data_size = nSize - 0x4;
		DecodeRound0(enc_data_ptr, enc_data_size, decode_type);
		DecodeRound1(sg_aTable, enc_data_ptr, enc_data_size, start_index);

		if (isMakeReadable)
		{
			pData[0] = 0xFF;
			pData[1] = 0xFF;
		}
	}
}