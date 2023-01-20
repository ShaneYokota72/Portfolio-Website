
        /* function marktime(){
            console.log(`{"time" : ${playerhtml.currentTime}, "lyrics": ${lyricsarray.pop()}}`)
        } */


        /* var lyricsdata = `아직도 너의 소리를 듣고
아직도 너의 손길을 느껴
오늘도 난 너의 흔적 안에 살았죠
아직도 너의 모습이 보여
아직도 너의 온기를 느껴
오늘도 난 너의 시간 안에 살았죠
길을 지나는 어떤 낯선 이의 모습 속에도
바람을 타고 쓸쓸히 춤추는 저 낙엽 위에도
뺨을 스치는 어느 저녁의 그 공기 속에도
내가 보고 듣고 느끼는 모든 것에 니가 있어 그래`; */
var title = "기억을 걷는 시간";
var artist = "넬(NELL)";
var musictitle = `${title} - ${artist}`
var lyricstimedata = [{"time" : 1.77863, "lyrics": "아직도 너의 소리를 듣고"},
{"time" : 7.081552, "lyrics": "아직도 너의 손길을 느껴"},
{"time" : 12.599053, "lyrics": "오늘도 난 너의 흔적 안에 살았죠"},
{"time" : 23.337033, "lyrics": "아직도 너의 모습이 보여"},
{"time" : 28.729257, "lyrics": "아직도 너의 온기를 느껴"},
{"time" : 34.105921, "lyrics": "오늘도 난 너의 시간 안에 살았죠"},
{"time" : 45.216465, "lyrics": "길을 지나는 어떤 낯선 이의 모습 속에도"},
{"time" : 51.008788, "lyrics": "바람을 타고 쓸쓸히 춤추는 저 낙엽 위에도"},
{"time" : 55.535228, "lyrics": "뺨을 스치는 어느 저녁의 그 공기 속에도"},
{"time" : 61.264631, "lyrics": "내가 보고 듣고 느끼는 모든 것에 니가 있어 그래"}]
/* var lyricsarray = lyricsdata.split("\n").reverse(); */
/* var lyricsdisplay = lyricsarray[k]; */
/* var k = ""; */

var titlehtml = document.getElementById("title");
var lyricshtml = document.getElementById("lyrics");
var playerhtml = document.getElementById("player");

//var experiment = [{"time" : 1.2}, {"time" : 4}]
//experiment[0].time

playerhtml.ontimeupdate = function(){lyricsdisplaygo()};
playerhtml.onplay = function(){displaytitle()};

function lyricsdisplaygo(){
    document.getElementById("lyrics").style.color = "black";
    for( var i = 0; i < lyricstimedata.length; i++){
        if(playerhtml.currentTime - lyricstimedata[i].time >= 0){
            lyricshtml.innerText = lyricstimedata[i].lyrics;
        }
    }
}
function displaytitle(){
    document.getElementById("title").style.color = "black";
    titlehtml.innerText = musictitle;
}

//document.getElementById("title").style.color