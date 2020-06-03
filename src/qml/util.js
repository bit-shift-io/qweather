function parse(xml) {
    var doc = xml.responseXML.documentElement;
    showRequestInfo("xhr length: " + doc.childNodes.length );

    for (var i = 0; i < doc.childNodes.length; ++i) {
        var child = doc.childNodes[i];

        for (var j = 0; j < child.childNodes.length; ++j) {
            if ( child.nodeName ===  "Cube") {

                var kid = child.childNodes[j];
                var length = kid.childNodes.length;

                for ( var k = 0; k < length; k ++) {
                    var cube = kid.childNodes[k];

                    if ( cube.nodeName === "Cube") {
                        var len = cube.attributes.length;
                        var currency = cube.attributes[0].nodeValue;
                        var rate = cube.attributes[1].nodeValue;
                        currencies.append({"currency": currency, "rate": parseFloat(rate)})
                    }
                }
            }
        }
    }
}
