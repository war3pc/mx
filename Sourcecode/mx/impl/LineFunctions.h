// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/utility/OptionalMembers.h"
#include "mx/api/LineData.h"
#include "mx/core/Enums.h"

namespace mx
{
    namespace impl
    {
        using LongDouble = long double;
        
        MX_ATTR_FUNC_OPTIONAL( hasLineType, HasLineType, bool, false );
        MX_ATTR_FUNC_OPTIONAL( lineType, LineType, core::LineType, core::LineType::solid );

        MX_ATTR_FUNC_OPTIONAL( hasLineEnd, HasLineEnd, bool, false );
        MX_ATTR_FUNC_OPTIONAL( lineEnd, LineEnd, core::LineEnd, core::LineEnd::none );
        
        MX_ATTR_FUNC_OPTIONAL( hasDashLength, HasDashLength, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( dashLength, DashLength, long double, 0.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasSpaceLength, HasSpaceLength, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( spaceLength, SpaceLength, long double, 0.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasEndLength, HasEndLength, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( endLength, EndLength, long double, 0.0L );
        
        template <typename ATTRIBUTES_TYPE>
        api::LineData getLineData( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::LineData lineData;
            Converter converter;
            
            if( checkHasLineType<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                lineData.lineType = converter.convert( checkLineType<ATTRIBUTES_TYPE>( &inAttributes ) );
            }
            
            if( checkHasLineEnd<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                lineData.lineEnd = converter.convert( checkLineEnd<ATTRIBUTES_TYPE>( &inAttributes ) );
            }
            
            if( checkHasDashLength( &inAttributes ) )
            {
                lineData.isDashLengthSpecified = true;
                lineData.dashLength = checkDashLength( &inAttributes );
            }
            
            if( checkHasSpaceLength( &inAttributes ) )
            {
                lineData.isSpaceLengthSpecified = true;
                lineData.spaceLength = checkSpaceLength( &inAttributes );
            }
            
            if( checkHasEndLength<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                lineData.isEndLengthSpecified = true;
                lineData.endLength = checkEndLength<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            return lineData;
        }
        
        MX_ATTR_SETFUNC_OPTIONAL( hasLineType, HasLineType, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( lineType, LineType, core::LineType, core::LineType::solid );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasLineEnd, HasLineEnd, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( lineEnd, LineEnd, core::LineEnd, core::LineEnd::none );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasSpaceLength, HasSpaceLength, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( spaceLength, SpaceLength, long double, 0.0L );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasDashLength, HasDashLength, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( dashLength, DashLength, long double, 0.0L );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasEndLength, HasEndLength, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( endLength, EndLength, long double, 0.0L );
        
        template <typename ATTRIBUTES_TYPE>
        void setAttributesFromLineData( const api::LineData& inLineData, ATTRIBUTES_TYPE& outAttributes )
        {
            Converter converter;
            if( lookForAndSetHasDashLength( inLineData.isDashLengthSpecified, &outAttributes ) )
            {
                lookForAndSetDashLength( inLineData.dashLength, &outAttributes );
            }
            
            if( lookForAndSetHasSpaceLength( inLineData.isSpaceLengthSpecified, &outAttributes ) )
            {
                lookForAndSetSpaceLength( inLineData.spaceLength, &outAttributes );
            }
            
            if( lookForAndSetHasLineEnd( inLineData.lineEnd != api::LineHook::unspecified, &outAttributes ) )
            {
                lookForAndSetLineEnd( converter.convert( inLineData.lineEnd ), &outAttributes );
            }
            
            if( lookForAndSetHasLineType( inLineData.lineType != api::LineType::unspecified, &outAttributes ) )
            {
                lookForAndSetLineType( converter.convert( inLineData.lineType ), &outAttributes );
            }
            
            if( lookForAndSetHasEndLength( inLineData.isEndLengthSpecified, &outAttributes ) )
            {
                lookForAndSetEndLength( inLineData.endLength, &outAttributes );
            }
        }
    }
}