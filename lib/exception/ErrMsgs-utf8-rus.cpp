#include "ftwd/errcodes.h"
namespace ftwd {
	const char* const ErrMsgs[] = {
		/* GlobalNamespace */
		/* Success */ "",
		/* globErrNullPtr */ "Передан нулевой указатель",
		/* globErrNullFilePtr */ "Получен нулевой указатель на файл",
		/* RefcounterBadRefCount */ "Ошибка счетчика ссылок",
		/* globErrOpenFile */ "Ошибка открытия файла",
		/* globErrReadingFile */ "Ошибка чтения из файла",
		/* globErrWrittingFile */ "Ошибка записи в файл",
		/* globErrBadAlloc */ "Ошибка выделения памяти(Возможно исчерпана оперативная память)",
		/* globErrNullBytesAlloc */ "Запрошено выделения 0 байт",
		/* globInvFileName */ "Такого файла или каталога не существует",
		/* globErrArrayOutOfRange */ "Выход за рамки массива",
		/* globErrInvalidValueType */ "Неверный тип аргумента",
		/* globErrBadArgs */ "Ошибка в переданных аргументах",

		/* encoding.h */
		/* EncErrUTF8NoLibfile */ "Не найдена библиотека символов UTF8",
		/* EncErrUTF8BadLibfile */ "Библиотека символов UTF8 повреждена или имеет не верный формат",
		/* EncErrInvalidUTF8Char */ "не могу найти запрошенный символ UTF8",
		/* EncErrUCS2NoLibfile */ "Не найдена библиотека символов Юникода(UCS2)",
		/* EncErrUCS2BadLibfile */ "Библиотека символов Юникода(UCS2) повреждена или имеет не верный формат",
		/* EncErrInvalidUCS2Char */ "не могу найти запрошенный символ Юникод(UCS2)",

		/* converter.h */
		/* ConvErrEncodingUndefined */  "Конвертер: не могу определить кодировку",
		/* ConvDefinedBomUTF8 */  "Конвертер: найден маркер 'UTF8'",
		/* ConvDefinedBomUCS2BE */  "Конвертер: найден маркер 'UCS2BE'",
		/* ConvDefinedBomUCS2LE */  "Конвертер: найден маркер 'UCS2LE'",
		/* ConvNoBOM */  "Конвертер: не известный байтовый маркер(BOM)",

		/* UnknownError */ "Неизвестная ошибка",
		/* errcodesSizeOffset*/ ""
	};
}
