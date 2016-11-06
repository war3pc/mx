// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        enum class LineType
        {
            unspecified,
            solid,
            dashed,
            dotted,
            wavy
        };

        enum class LineHook
        {
            unspecified,
            down,
            up,
            both,
            arrow,
            none
        };
        
        struct LineData
        {
            LineType lineType;
            LineHook lineEnd;
            bool isEndLengthSpecified;
            long double endLength;
            bool isDashLengthSpecified;
            long double dashLength;
            bool isSpaceLengthSpecified;
            long double spaceLength;
            
            inline bool isSpecified() const
            {
                return lineType != LineType::unspecified ||
                isDashLengthSpecified ||
                isSpaceLengthSpecified;
            }
            
            LineData()
            : lineType{ LineType::unspecified }
            , lineEnd{ LineHook::unspecified }
            , isEndLengthSpecified{ false }
            , endLength{ 0.0 }
            , isDashLengthSpecified{ false }
            , dashLength{ 0.0 }
            , isSpaceLengthSpecified{ false }
            , spaceLength{ 0.0 }
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( LineData )
        MXAPI_EQUALS_MEMBER( lineType )
        MXAPI_EQUALS_MEMBER( lineEnd )
        MXAPI_EQUALS_MEMBER( isEndLengthSpecified )
        MXAPI_EQUALS_MEMBER( endLength )
        MXAPI_EQUALS_MEMBER( isDashLengthSpecified )
        MXAPI_EQUALS_MEMBER( dashLength )
        MXAPI_EQUALS_MEMBER( isSpaceLengthSpecified )
        MXAPI_EQUALS_MEMBER( spaceLength )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( LineData );
    }
}