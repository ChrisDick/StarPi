
  // helper function: log message to screen
        function status(msg) {
            document.getElementById('status').textContent = msg;
        };
        
        function declination(msg) {
            document.getElementById('declination').textContent = msg;
        };
        
        function right_ascension(msg) {
            document.getElementById('right_ascension').textContent = msg;
        };
        
        function Pitch(msg) {
            document.getElementById('Altitude').textContent = msg;
        };
        
        function Azimuth(msg) {
            document.getElementById('Azimuth').textContent = msg;
        };
        
        function Lat(msg) {
            document.getElementById('Latitude').textContent = msg;
        };
        
        function longitude(msg) {
            document.getElementById('longitude').textContent = msg;
        };
        
        function Height(msg) {
            document.getElementById('Height').textContent = msg;
        };
        
        function MagneticDeclination(msg) {
            document.getElementById('MagneticDeclination').textContent = msg;
        };
        
        function TrueHeading(msg) {
            document.getElementById('TrueHeading').textContent = msg;
        };
        
        function MagneticHeading(msg) {
            document.getElementById('MagneticHeading').textContent = msg;
        };
        
        function Day(msg) {
            document.getElementById('Day').textContent = msg;
        };
        
        function Month(msg) {
            document.getElementById('Month').textContent = msg;
        };
        
        function Year(msg) {
            document.getElementById('Year').textContent = msg;
        };
        
        function TimeUpdate(msg) {
            document.getElementById('TimeDisplay').textContent = msg;
        };
        
        function summertime(msg) {
            document.getElementById('SummerTime').textContent = msg;
        };
        
        function lst(msg) {
            document.getElementById('lst').textContent = msg;
        };
        
        function debug(msg) {
            document.getElementById('debug').textContent = msg;
        };
        
        function GMTMinutes(msg) {
            document.getElementById('gmtminutes').textContent = msg;
        };
        
        function GMTSeconds(msg) {
            document.getElementById('gmtseconds').textContent = msg;
        };
        
        function LocalSiderealMinutes(msg) {
            document.getElementById('localsiderealminutes').textContent = msg;
        };
        
        function LocalSiderealSeconds(msg) {
            document.getElementById('localsiderealseconds').textContent = msg;
        };
        
        function RAMinutes(msg) {
            document.getElementById('raminutes').textContent = msg;
        };
        
        function RASeconds(msg) {
            document.getElementById('raseconds').textContent = msg;
        };
        
        function DeclinationMinutes(msg) {
            document.getElementById('declinationminutes').textContent = msg;
        };
        
        function DeclinationSeconds(msg) {
            document.getElementById('declinationseconds').textContent = msg;
        };
        
        function JulianDate(msg) {
            document.getElementById('juliandate').textContent = msg;
        };

        // setup websocket with callbacks
        var ws = new WebSocket('ws://SPACECAM:1234/');
        ws.onopen = function() {
            status('CONNECT');
        };
        ws.onclose = function() {
            status('DISCONNECT');
        };
        ws.onmessage = function(event) {
        var text = event.data;
        if (text.indexOf("DECH") != -1)
        {
            declination(text.slice(4));
        }
        if (text.indexOf("RAH ") != -1)
        {
            right_ascension(text.slice(4));
        }
        if (text.indexOf("Pitc") != -1)
        {
            Pitch(text.slice(4));
        }
        if (text.indexOf("Azim") != -1)
        {
            Azimuth(text.slice(4));
        }
        if (text.indexOf("Lati") != -1)
        {
            Lat(text.slice(4));
        }
        if (text.indexOf("Long") != -1)
        {
            longitude(text.slice(4));
        }
        if (text.indexOf("MagD") != -1)
        {
            MagneticDeclination(text.slice(4));
        }
        if (text.indexOf("MagH") != -1)
        {
            MagneticHeading(text.slice(4));
        }
        if (text.indexOf("High") != -1)
        {
            Height(text.slice(4));
        }
        if (text.indexOf("TruH") != -1)
        {
            TrueHeading(text.slice(4));
        }
        if (text.indexOf("GMTM") != -1)
        {
            Month(text.slice(4));
        }
        if (text.indexOf("GMTD") != -1)
        {
            Day(text.slice(4));
        }
        if (text.indexOf("GMTY") != -1)
        {
            Year(text.slice(4));
        }
        if (text.indexOf("GMTH") != -1)
        {
            TimeUpdate(text.slice(4));
        }
        if (text.indexOf("BST ") != -1)
        {
            summertime(text.slice(4));
        }
        if (text.indexOf("LSTH") != -1)
        {
            lst(text.slice(4));
        }
        if (text.indexOf("DEBU") != -1)
        {
            debug(text.slice(4));
        }
        if (text.indexOf("GMTm") != -1)
        {
            GMTMinutes(text.slice(4));
        }
        if (text.indexOf("GMTS") != -1)
        {
            GMTSeconds(text.slice(4));
        }
        if (text.indexOf("LSTm") != -1)
        {
            LocalSiderealMinutes(text.slice(4));
        }
        if (text.indexOf("LSTS") != -1)
        {
            LocalSiderealSeconds(text.slice(4));
        }
        if (text.indexOf("RAm ") != -1)
        {
            RAMinutes(text.slice(4));
        }
        if (text.indexOf("RAS ") != -1)
        {
            RASeconds(text.slice(4));
        }
        if (text.indexOf("DECm") != -1)
        {
            DeclinationMinutes(text.slice(4));
        }
        if (text.indexOf("DECS") != -1)
        {
            DeclinationSeconds(text.slice(4));
        }
        if (text.indexOf("JDAT") != -1)
        {
            JulianDate(text.slice(4));
        }
    
        document.getElementById('recieved').textContent = text;
    };
    function sendHeight()
    {
        var message = "WHIH " + document.getElementById('UserHeight').value + "\n" ;
        ws.send( message );
        document.getElementById('sent').textContent = "Sent: " + message;
    };
    function sendLatitude()
    {
        /* make this convert to HMS format */
        var message = "Lati " + document.getElementById('UserLatitude').value + "\n" ;
        ws.send( message );
        document.getElementById('sent').textContent = "Sent: " + message;
    };
    function sendLongitude()
    {
        /* make this convert to HMS format */
        var message = "Long " + document.getElementById('UserLongitude').value + "\n" ;
        ws.send( message );
        document.getElementById('sent').textContent = "Sent: " + message;
    };

    function sendSetUser()
    {
        var message = "GPSS 1\n" ;
        ws.send( message );
        document.getElementById('sent').textContent = "Sent: " + message;
    };
    
    function sendSetGps()
    {
        var message = "GPSS 0\n" ;
        ws.send( message );
        document.getElementById('sent').textContent = "Sent: " + message;
    };
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    