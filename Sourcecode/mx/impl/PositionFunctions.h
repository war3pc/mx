// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/PositionData.h"
#include "mx/utility/OptionalMembers.h"
#include "mx/impl/Converter.h"

namespace mx
{
    namespace impl
    {
        using LongDouble = long double;
        
        MX_ATTR_FUNC_OPTIONAL( hasDefaultX, HasDefaultX, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( defaultX, DefaultX, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasDefaultY, HasDefaultY, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( defaultY, DefaultY, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasRelativeX, HasRelativeX, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( relativeX, RelativeX, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasRelativeY, HasRelativeY, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( relativeY, RelativeY, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasHalign, HasHalign, bool, false );
        MX_ATTR_FUNC_OPTIONAL( halign, Halign, core::LeftCenterRight, core::LeftCenterRight::left );

        MX_ATTR_FUNC_OPTIONAL( hasValign, HasValign, bool, false );
        MX_ATTR_FUNC_OPTIONAL( valign, Valign, core::Valign, core::Valign::baseline );

        MX_ATTR_FUNC_OPTIONAL( hasPlacement, HasPlacement, bool, false );
        MX_ATTR_FUNC_OPTIONAL( placement, Placement, core::AboveBelow, core::AboveBelow::above );
        
        template <typename ATTRIBUTES_TYPE>
        api::PositionData createPositionData( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::PositionData outPositionData;
            
            if( checkHasDefaultX( &inAttributes ) )
            {
                outPositionData.hasDefaultX = true;
                outPositionData.defaultX = checkDefaultX( &inAttributes );
            }
            
            if( checkHasDefaultY( &inAttributes ) )
            {
                outPositionData.hasDefaultY = true;
                outPositionData.defaultY = checkDefaultY( &inAttributes );
            }
            
            if( checkHasRelativeX( &inAttributes ) )
            {
                outPositionData.hasRelativeX = true;
                outPositionData.relativeX = checkRelativeX( &inAttributes );
            }
            
            if( checkHasRelativeY( &inAttributes ) )
            {
                outPositionData.hasRelativeY = true;
                outPositionData.relativeY = checkRelativeY( &inAttributes );
            }
            
            impl::Converter converter;
            
            if( checkHasHalign( &inAttributes ) )
            {
                outPositionData.horizontalAlignmnet = converter.convert( checkHalign( &inAttributes ) );
            }
            else
            {
                outPositionData.horizontalAlignmnet = api::HorizontalAlignment::unspecified;
                
            }
            
            if( checkHasValign( &inAttributes ) )
            {
                outPositionData.verticalAlignment = converter.convert( checkValign( &inAttributes ) );
            }
            else
            {
                outPositionData.verticalAlignment = api::VerticalAlignment::unspecified;
            }

            if( checkHasPlacement( &inAttributes ) )
            {
                outPositionData.placement = converter.convert( checkPlacement( &inAttributes ) );
            }
            else
            {
                outPositionData.placement = api::Placement::unspecified;
            }
            
            return outPositionData;
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        void createPositionData( const api::PositionData& inPositionData, ATTRIBUTES_TYPE& outAttributes )
        {
            if( inPositionData.hasDefaultX )
            {
                outAttributes.hasDefaultX = true;
                outAttributes.defaultX.setValue( inPositionData.defaultX );
            }
            
            if( inPositionData.hasDefaultY )
            {
                outAttributes.hasDefaultY = true;
                outAttributes.defaultY.setValue( inPositionData.defaultY );
            }
            
            if( inPositionData.hasRelativeX )
            {
                outAttributes.hasRelativeX = true;
                outAttributes.relativeX.setValue( inPositionData.relativeX );
            }
            
            if( inPositionData.hasRelativeY )
            {
                outAttributes.hasRelativeY = true;
                outAttributes.relativeY.setValue( inPositionData.relativeY );
            }
            
            switch ( inPositionData.horizontalAlignmnet )
            {
                case api::HorizontalAlignment::unspecified:
                {
                    outAttributes.hasHalign = false;
                    break;
                }
                case api::HorizontalAlignment::left:
                {
                    outAttributes.hasHalign = true;
                    outAttributes.halign = core::LeftCenterRight::left;
                    break;
                }
                case api::HorizontalAlignment::center:
                {
                    outAttributes.hasHalign = true;
                    outAttributes.halign = core::LeftCenterRight::center;
                    break;
                }
                case api::HorizontalAlignment::right:
                {
                    outAttributes.hasHalign = true;
                    outAttributes.halign = core::LeftCenterRight::right;
                    break;
                }
                default:
                {
                    outAttributes.hasHalign = false;
                    break;
                }
            }
        }
    }
}