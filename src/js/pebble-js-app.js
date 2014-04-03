function getNextRace() {
    var e, a = new XMLHttpRequest();
    a.open("GET", "http://ergast.com/api/f1/" + currSeason + "/" + raceChoice + ".json", !0), 
    a.onload = function() {
        e = JSON.parse(a.responseText), series = e.MRData.series, season = e.MRData.RaceTable.season;
        e.MRData.RaceTable.round, e.MRData.RaceTable.Races[0].raceName, e.MRData.RaceTable.Races[0].Circuit.Location.locality;
        date = e.MRData.RaceTable.Races[0].date;
        var t = date;
        time = e.MRData.RaceTable.Races[0].time;
        var n = time;
        n = n.replace("Z", " UTC");
        {
            var r = " ", c = r.concat(n), s = t.concat(c);
            e.MRData.RaceTable.Races[0].Circuit.circuitName, e.MRData.RaceTable.Races[0].Circuit.Location.country, 
            new Date().getTimezoneOffset() / 60;
        }
        raceDate = new Date(date), raceEpoch = date + "";
        var o = function(e) {
            var a = /^\s*(\d{4})-(\d\d)-(\d\d)\s+(\d\d):(\d\d):(\d\d)\s+UTC\s*$/, t = ("" + e).match(a);
            return t ? Date.UTC(t[1], t[2] - 1, t[3], t[4], t[5], t[6]) : void 0;
        };
        date = o(s) / 1e3, sendMessage();
    }, a.send(null);
}

function sendMessage() {
    Pebble.sendAppMessage({
        series: "F1 " + season,
        season: season,
        round: "Round " + round,
        round_number: round,
        raceName: raceName,
        locality: circuitName + "\n" + locality + ", " + country,
        date: date,
        time: time,
        offset: offset,
        max_round: max_round
    });
}

var season, raceName, circuitName, locality, date, time, offset, round, max_round, country, raceChoice = localStorage.getItem("raceChoice");

raceChoice || (raceChoice = "next");

var currSeason = localStorage.getItem("currSeason");

currSeason || (currSeason = "current"), Pebble.addEventListener("ready", function() {
    getNextRace();
}), Pebble.addEventListener("appmessage", function(e) {
    e.payload.request_key && (raceChoice = e.payload.request_key, localStorage.setItem("raceChoice", raceChoice)), 
    e.payload.season_req_key && (currSeason = e.payload.season_req_key, localStorage.setItem("currSeason", currSeason)), 
    getNextRace(), sendMessage();
});

var MessageQueue = function() {
    function e() {
        s = [], o = !1;
    }
    function a(e, a, t) {
        return n(e) ? (s.push({
            message: e,
            ack: a || null,
            nack: t || null,
            attempts: 0
        }), setTimeout(function() {
            r();
        }, 1), !0) : !1;
    }
    function t() {
        return s.length;
    }
    function n(e) {
        function a(e) {
            switch (typeof e) {
              case "string":
                return !0;

              case "number":
                return !0;

              case "object":
                if ("[object Array]" == toString.call(e)) return !0;
            }
            return !1;
        }
        if (e !== Object(e)) return !1;
        var t = Object.keys(e);
        if (!t.length) return !1;
        for (var n = 0; n < t.length; n += 1) {
            var r = /^[0-9a-zA-Z-_]*$/.test(t[n]);
            if (!r) return !1;
            var c = e[t[n]];
            if (!a(c)) return !1;
        }
        return !0;
    }
    function r() {
        function e() {
            clearTimeout(u), setTimeout(function() {
                o = !1, r();
            }, 200), n.ack && n.ack.apply(null, arguments);
        }
        function a() {
            clearTimeout(u), n.attempts < c ? (s.unshift(n), setTimeout(function() {
                o = !1, r();
            }, 200 * n.attempts)) : n.nack && n.nack.apply(null, arguments);
        }
        function t() {
            setTimeout(function() {
                o = !1, r();
            }, 1e3), n.ack && n.ack.apply(null, arguments);
        }
        if (!o) {
            var n = s.shift();
            n && (n.attempts += 1, o = !0, Pebble.sendAppMessage(n.message, e, a), u = setTimeout(function() {
                t();
            }, 1e3));
        }
    }
    var c = 5, s = [], o = !1, u = null;
    return {
        reset: e,
        sendAppMessage: a,
        size: t
    };
}();