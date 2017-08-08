#include "ftwd/errcodes.h"

namespace ftwd {
    const char * const ErrMsgs[] = {
        /* GlobalNamespace */
        /* Success */ "",
        /* globErrNullPtr */ "Empty pointer given",
        /* globErrNullFilePtr */ "Empty filepointer given",
        /* RefcounterBadRefCount */ "Invalid reference count",
        /* globErrOpenFile */ "Error opening file",
        /* globErrReadingFile */ "Error reading file",
        /* globErrWrittingFile */ "Error writting file",
        /* globErrBadAlloc */ "Error Bad allocation(out of free mrmory)",
        /* globErrNullBytesAlloc */ "Error invalid allocation bytes count",
        /* globInvFileName */ "Error No such file or directory",
        /* globErrArrayOutOfRange */ "Error out of array range",
        /* globErrInvalidValueType */ "Error invalid type given",
		/* globErrBadArgs */ "Error: Invalid arguments values given",

        /* encoding.h */
        /* EncErrUTF8NoLibfile */ "UTF8 libfile is missing",
        /* EncErrUTF8BadLibfile */ "UTF8 libfile is corrupt",
        /* EncErrInvalidUTF8Char */ "Can't find UTF8 character in libfile",
        /* EncErrUCS2NoLibfile */ "UCS2 libfile is missing",
        /* EncErrUCS2BadLibfile */ "UCS2 libfile is corrupt",
        /* EncErrInvalidUCS2Char */ "Can't find UCS2 character in libfile",

        /* converter.h */
        /* ConvErrEncodingUndefined */  "Converter: Unknown file encoding",
        /* ConvDefinedBomUTF8 */  "Converter: found BOM 'UTF8'",
        /* ConvDefinedBomUCS2BE */  "Converter: found BOM 'UCS2BE'",
        /* ConvDefinedBomUCS2LE */  "Converter: found BOM 'UCS2LE'",
        /* ConvNoBOM */  "Converter: Unknown Byte Marker",

        /* UnknownError */ "Unknown error",
        /* errcodesSizeOffset */ ""
    };
}

