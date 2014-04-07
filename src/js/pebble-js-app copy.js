// Function to send a message to the Pebble using AppMessage API
/*jshint smarttabs:true*/


var season, raceName, circuitName, locality, date, time, offset, round, max_round, country;

var raceChoice = localStorage.getItem("raceChoice");
console.log("var raceChoice create");

if (!raceChoice) {
    console.log("if !raceChoice then raceChoice = next");
    raceChoice = "next";
}
var currSeason = localStorage.getItem("currSeason");
if (!currSeason) {
	currSeason = "current";
}

function getNextRace() {
    var response;
    var req = new XMLHttpRequest();
	
	req.open('GET', "http://ergast.com/api/f1/" + currSeason + "/" + raceChoice + ".json", true);
	console.log("opening http://ergast.com/api/f1/" + currSeason + "/" + raceChoice + ".json");
    req.onload = function(e) {
        
        console.log("http://ergast.com/api/f1/" + currSeason + "/" + raceChoice + ".json req.onload complete!");
        response = JSON.parse(req.responseText);
        //			console.log(req.responseText);
        series = response.MRData.series;
        season = response.MRData.RaceTable.season;
        var round = response.MRData.RaceTable.round;
        var raceName = response.MRData.RaceTable.Races[0].raceName;
        var locality = response.MRData.RaceTable.Races[0].Circuit.Location.locality;
        date = response.MRData.RaceTable.Races[0].date;
        var date_string = date;
        time = response.MRData.RaceTable.Races[0].time;
        var time_string = time;
        time_string = time_string.replace("Z", " UTC");
        var space_string = " ";
        var race_format_mid_string = space_string.concat(time_string);
        var race_time_date_string = date_string.concat(race_format_mid_string);
        var circuitName = response.MRData.RaceTable.Races[0].Circuit.circuitName;
        var country = response.MRData.RaceTable.Races[0].Circuit.Location.country;
        var offset = new Date().getTimezoneOffset() / 60;
        raceDate = new Date(date);
        raceEpoch = (date + "");
        
        var getEpochMillis = function(dateStr) {
            var r = /^\s*(\d{4})-(\d\d)-(\d\d)\s+(\d\d):(\d\d):(\d\d)\s+UTC\s*$/,
            m = (""+dateStr).match(r);
            return (m) ? Date.UTC(m[1], m[2]-1, m[3], m[4], m[5], m[6]) : undefined;
        };
        date = getEpochMillis(race_time_date_string) / 1000;
        
        console.log(season + time + time_string + country + offset + circuitName + round + raceName + locality);
        console.log("sending message");
        sendMessage();
        //			Pebble.sendAppMessage({
        //				"season":season,
        //				})
        //			sendMessage();
        
        /*			MessageQueue.sendAppMessage({
         "series":"F1 " + season,
         "season":season,
         "round":"Round " + round,
         "round_number":round,
         "raceName":raceName,
         });
         */
        /*			MessageQueue.sendAppMessage({
         "locality":circuitName + "\n" + locality + ", " + country,
         "date":date,
         "time":time,
         "offset":offset,
         "max_round":max_round,
         });  */
    };
	
    /*		req.open('GET', "http://ergast.com/api/f1/" + currSeason + ".json", true);
     //		console.log("opening http://ergast.com/api/f1/" + currSeason + ".json");
     req.onload = function(e) {
     console.log("http://ergast.com/api/f1/" + currSeason + ".json req.onload complete!");
     //		console.log(req.responseText);
     
     response = JSON.parse(req.responseText);
     var max_round;
     max_round = response.MRData.RaceTable.total;
     
     MessageQueue.sendAppMessage({
     "max_round":max_round,
     });
     
     }; */
	
	
	req.send(null);
}

Pebble.addEventListener("ready",
                        function(e) {
                        getNextRace();
                        console.log("ready event, var raceChoice = " + raceChoice);
                        });

Pebble.addEventListener("appmessage",
                        function(e) {
                        console.log("appmessage in, before payload check");
                        console.log("var (func e) raceChoice = " + raceChoice);
                        console.log("var (func e) currSeason = " + currSeason);
                        console.log("checking payload(s)");
                        if (e.payload.request_key) {
                        raceChoice = e.payload.request_key;
                        localStorage.setItem("raceChoice", raceChoice);
                        console.log("var palyload raceChoice = " + raceChoice);
                        //                            getNextRace(raceChoice);
						} else {}
						if (e.payload.season_req_key) {
                        console.log("if (e.payload.season_request_key) == true");
                        currSeason = e.payload.season_req_key;
                        localStorage.setItem("currSeason", currSeason);
                        console.log("var payload currSeason = " + currSeason);
                        console.log("e.payload.season_req_key = " + e.payload.season_req_key);
                        
						} else {}
						getNextRace();
						sendMessage();
                        });


var MessageQueue = function() {
    function t() {
        s = [], c = !1;
    }
    function e(t, e, n) {
        return u(t) ? (s.push({
                              message: t,
                              ack: e || null,
                              nack: n || null,
                              attempts: 0
                              }), setTimeout(function() {
                                             r();
                                             }, 1), !0) : !1;
    }
    function n() {
        return s.length;
    }
    function u(t) {
        function e(t) {
            switch (typeof t) {
                case "string":
                    return !0;
                    
                case "number":
                    return !0;
                    
                case "object":
                    if ("[object Array]" == toString.call(t)) return !0;
            }
            return !1;
        }
        if (t !== Object(t)) return !1;
        var n = Object.keys(t);
        if (!n.length) return !1;
        for (var u = 0; u < n.length; u += 1) {
            var r = /^[0-9a-zA-Z-_]*$/.test(n[u]);
            if (!r) return !1;
            var a = t[n[u]];
            if (!e(a)) return !1;
        }
        return !0;
    }
    function r() {
        function t() {
            clearTimeout(i), setTimeout(function() {
                                        c = !1, r();
                                        }, 200), u.ack && u.ack.apply(null, arguments);
        }
        function e() {
            clearTimeout(i), u.attempts < a ? (s.unshift(u), setTimeout(function() {
                                                                        c = !1, r();
                                                                        }, 200 * u.attempts)) : u.nack && u.nack.apply(null, arguments);
        }
        function n() {
            setTimeout(function() {
                       c = !1, r();
                       }, 1e3), u.ack && u.ack.apply(null, arguments);
        }
        if (!c) {
            var u = s.shift();
            u && (u.attempts += 1, c = !0, Pebble.sendAppMessage(u.message, t, e), i = setTimeout(function() {
                                                                                                  n();
                                                                                                  }, 1e3));
        }
    }
    var a = 5, s = [], c = !1, i = null;
    return {
    reset: t,
    sendAppMessage: e,
    size: n
    };
}();



function sendMessage() {
	console.log("sendMessage called .... are values below?"); 			
	console.log(season + time + time_string + country + offset + circuitName + round + raceName + locality); 
	Pebble.sendAppMessage({
                          "series":"F1 " + season,
                          "season":season,					 
                          "round":"Round " + round, // + ": " + raceName,// + ", " + locality + ", " + country,
                          "round_number":round, 
                          "raceName":raceName,
                          "locality":circuitName + "\n" + locality + ", " + country, 
                          "date":date, 
                          "time":time, 
                          "offset":offset,
                          "max_round":max_round, 
                          });
}