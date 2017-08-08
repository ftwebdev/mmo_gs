#include "ftwd/errcodes.h"
namespace ftwd {
	const char* const ErrMsgs[] = {
		/* GlobalNamespace */
		/* Success */ "",
		/* globErrNullPtr */ "������� ������� ���������",
		/* globErrNullFilePtr */ "������� ������� ��������� �� ����",
		/* RefcounterBadRefCount */ "������ �������� ������",
		/* globErrOpenFile */ "������ �������� �����",
		/* globErrReadingFile */ "������ ������ �� �����",
		/* globErrWrittingFile */ "������ ������ � ����",
		/* globErrBadAlloc */ "������ ��������� ������(�������� ��������� ����������� ������)",
		/* globErrNullBytesAlloc */ "��������� ��������� 0 ����",
		/* globInvFileName */ "������ ����� ��� �������� �� ����������",
		/* globErrArrayOutOfRange */ "����� �� ����� �������",
		/* globErrInvalidValueType */ "�������� ��� ���������",
		/* globErrBadArgs */ "������ � ���������� ����������",

		/* encoding.h */
		/* EncErrUTF8NoLibfile */ "�� ������� ���������� �������� UTF8",
		/* EncErrUTF8BadLibfile */ "���������� �������� UTF8 ���������� ��� ����� �� ������ ������",
		/* EncErrInvalidUTF8Char */ "�� ���� ����� ����������� ������ UTF8",
		/* EncErrUCS2NoLibfile */ "�� ������� ���������� �������� �������(UCS2)",
		/* EncErrUCS2BadLibfile */ "���������� �������� �������(UCS2) ���������� ��� ����� �� ������ ������",
		/* EncErrInvalidUCS2Char */ "�� ���� ����� ����������� ������ ������(UCS2)",

		/* converter.h */
		/* ConvErrEncodingUndefined */  "���������: �� ���� ���������� ���������",
		/* ConvDefinedBomUTF8 */  "���������: ������ ������ 'UTF8'",
		/* ConvDefinedBomUCS2BE */  "���������: ������ ������ 'UCS2BE'",
		/* ConvDefinedBomUCS2LE */  "���������: ������ ������ 'UCS2LE'",
		/* ConvNoBOM */  "���������: �� ��������� �������� ������(BOM)",

		/* UnknownError */ "����������� ������",
		/* errcodesSizeOffset*/ ""
	};
}
