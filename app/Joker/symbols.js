.import PhImport 1.0 as Ph
.import "qrc:/fonts/fontawesome.js" as FontAwesome

function symbolFromDetectType(type)
{
    console.log("symbol from type " + type)
    if (type === Ph.PhStripDetect.Unknown) {
        return FontAwesome.Icon.bookmark; // or tag ? question ? map_marker ?
    } else if (type === Ph.PhStripDetect.Advance) {
        return FontAwesome.Icon.bullseye;
    } else if (type === Ph.PhStripDetect.Aperture) {
        return FontAwesome.Icon.dot_circle_o;
    } else if (type === Ph.PhStripDetect.Bowl) {
        return FontAwesome.Icon.circle_o;
    } else if (type === Ph.PhStripDetect.Dental) {
        return FontAwesome.Icon.pause_circle_o;
    } else if (type === Ph.PhStripDetect.Labial) {
        return FontAwesome.Icon.random; // or h_square ?
    } else if (type === Ph.PhStripDetect.MouthClosed) {
        return FontAwesome.Icon.compress;
    } else if (type === Ph.PhStripDetect.MouthOpen) {
        return FontAwesome.Icon.expand;
    } else {
        return FontAwesome.Icon.exclamation_triangle;
    }
}
