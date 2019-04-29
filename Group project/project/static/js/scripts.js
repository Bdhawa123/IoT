var requestURL = 'http://api.openweathermap.org/data/2.5/weather?q=Hawthorn,au&units=metric&APPID=aa10039e6d7af9de2abf440fdd78bbcd';
var request = new XMLHttpRequest();
request.open('GET', requestURL);
request.responseType = 'json';
request.send();

request.onload = function() {
  var weather = request.response;
  document.getElementById("outsideTemp").innerHTML = weather.main.temp;
}
