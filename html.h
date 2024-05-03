const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<head>
  <title>GTN-1000 Monitor</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:icon.jpg">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p { font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #50B8B4; color: white; font-size: 1rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .reading { font-size: 1.4rem; }
  </style>

  <script>
  //const dataJson = {"bt_v":28.1,"bt_a":2,"pw_req":50,"mode":"","error":"","temp":32,"ac_v":229,"ac_hz":49};
  
  function updateData(data) {
    let pwOut = (data.bt_v*data.bt_a*0.865).toFixed(0);
    document.getElementById("power").innerHTML = data.pw_req.toFixed(0);
    document.getElementById("pwOut").innerHTML = pwOut;
    document.getElementById("btVlt").innerHTML = data.bt_v.toFixed(1);
    document.getElementById("btAmp").innerHTML = data.bt_a.toFixed(1);
    document.getElementById("temp").innerHTML = data.temp.toFixed(1);
    document.getElementById("acVlt").innerHTML = data.ac_v.toFixed(0);
  }

  function eventSetup() {
    if (!!window.EventSource) {
      var source = new EventSource('/events');
      
      source.addEventListener('open', function(e) {
        console.log("Events Connected");
      }, false);

      source.addEventListener('data', function(e) {
        console.log("message", e.data);
        let data = JSON.parse(e.data) ? STATES = JSON.parse(e.data) : {};
        updateData(data);
      }, false);

      source.addEventListener('error', function(e) {
        if (e.target.readyState != EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      }, false);
      
      source.addEventListener('message', function(e) {
        console.log("message", e.data);
      }, false);
    }
  }


  onload = () => {
    eventSetup();
    //updateData(dataJson);
  }
  
  </script>

</head>
<body>
  <div class="topnav">
    <h1>SOLAR MONITOR</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p><i class="fas fa-plug" style="color:#C74042;"></i> CONSUMO </p><p><span class="reading"><span id="power">%POWER%</span> W</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-sun" style="color:#e1e437;"></i> POWER OUT </p><p><span class="reading"><span id="pwOut">%POWEROUT%</span> W</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-car-battery" style="color:#00add6;"></i> BAT VOLTS</p><p><span class="reading"><span id="btVlt">%BTVOLTS%</span> V</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-car-battery" style="color:#00add6;"></i> BAT CURRENT</p><p><span class="reading"><span id="btAmp">%BTAMP%</span> A</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-thermometer-half" style="color:#059e8a;"></i> TEMPERATURE</p><p><span class="reading"><span id="temp">%TEMPERATURE%</span> &deg;C</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-broadcast-tower" style="color:#C74042;"></i> AC VOLTS</p><p><span class="reading"><span id="acVlt">%ACVOLTS%</span> V</span></p>
      </div>
    </div>
  </div>

</body>
</html>)rawliteral";