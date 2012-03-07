/* Silde widget
 * Main widget for CaptainAdhoc's UI. This is a widget that can display its
 * children as slides.
 */

import QtQuick 1.1

Item {
    id: slideWidget

    default property alias content: stack.children

    property int current: 0

    signal slideChanged( string title )

    signal gotoNextSlide
    signal gotoPrevSlide

    onGotoNextSlide: showNextSlide()
    onGotoPrevSlide: showPrevSlide()

    onSlideChanged: { console.log( "new slide title : " + title ) }

    Component.onCompleted: {
        if( stack.children.length > 0 )
        {
            // check if current current value is acceptable.
            // if not, it is reset to 0.
            if( current >= stack.children.length  || current < 0 )
                current = 0;

            for( var i = 0 ; i < stack.children.length ; i++ )
            {
                if( i !== current )
                    stack.children[ i ].x = - slideWidget.width;
                else
                    stack.children[ i ].x = 0
            }

            slideWidget.slideChanged( stack.children[ current ].title );
        }
    }

    PropertyAnimation {
        id: currentViewAnimation
        duration : 500
        property: "x"
        easing.type: Easing.InQuad
    }

    PropertyAnimation {
        id: nextViewAnimation
        duration : 500
        property: "x"
        easing.type: Easing.InQuad
    }

    Item {
        id: stack
        anchors.fill: parent
    }

    function showNextSlide(){
        var currView = content[ current ];
        var nextIdx  = getNextSlideIdx();
        var nextView = content[ nextIdx ];

        current = nextIdx;

        changeSlide( currView, nextView, false );
    }

    function showPrevSlide(){
        var currView = content[ current ];
        var nextIdx  = getPrevSlideIdx();
        var nextView = content[ nextIdx ];

        current = nextIdx;

        changeSlide( currView, nextView, true );
    }

    function changeSlide( currView, nextView, leftToRight ){
        if( currentViewAnimation.running )
            currentViewAnimation.complete();

        if( nextViewAnimation.running )
            nextViewAnimation.complete();

        currentViewAnimation.target = currView;
        currentViewAnimation.to     = leftToRight ? slideWidget.width : - slideWidget.width;
        currentViewAnimation.start();

        nextViewAnimation.target = nextView;
        nextViewAnimation.from   = leftToRight ? - slideWidget.width : slideWidget.width;
        nextViewAnimation.to     = -0; // no idea why the "-" is here but it was in the example
        nextViewAnimation.start();

        slideWidget.slideChanged( currView.title )
    }

    function getNextSlideIdx(){
        return ( current < content.length - 1 ? current + 1 : 0 );
    }

    function getPrevSlideIdx(){
        return ( current > 0 ? current - 1 : content.length - 1 );
    }

    function getCurrentSlideTitle(){
        return content[ current ].title;
    }

    function getPrevSlideTitle(){
        return content[ getPrevSlideIdx() ].title;
    }

    function getNextSlideTitle(){
        return content[ getNextSlideIdx() ].title;
    }
}
