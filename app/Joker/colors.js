.import PhImport 1.0 as Ph

function colorFromDetectType(type)
{
    console.log("color from type " + type)
    if (type === Ph.PhStripDetect.Unknown) {
        return "lightgrey";
    } else if (type === Ph.PhStripDetect.Advance) {
        return "thistle";
    } else if (type === Ph.PhStripDetect.Aperture) {
        return "pink";
    } else if (type === Ph.PhStripDetect.Bowl) {
        return "salmon";
    } else if (type === Ph.PhStripDetect.Dental) {
        return "ivory";
    } else if (type === Ph.PhStripDetect.Labial) {
        return "skyblue";
    } else if (type === Ph.PhStripDetect.MouthClosed) {
        return "lightpink";
    } else if (type === Ph.PhStripDetect.MouthOpen) {
        return "darkorchid";
    } else {
        return "yellow";
    }
}
