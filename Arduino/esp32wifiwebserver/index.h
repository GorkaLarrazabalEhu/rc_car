
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>El Gorki carro</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <style>
        :root {
            --main-bg-color: #9e2c9c;
            --button-bg-color: white;
            --button-hover-color: grey;
            --button-active-bg-color: #007399;
            --button-active-color: white;
            --slider-bg-color: #dd3eda;
            --slider-thumb-color: #ffffff;
            --font-family: 'Times', serif;
            --font-family-button: 'appaji ambarisha darbha', sans-serif;
        }

        body {
            font-family: var(--font-family);
            text-align: center;
            margin: 0;
            padding-top: 30px;
            width: 100%;
            user-select: none;
            background-color: var(--main-bg-color);
            color: white;
        }

        .button {
            padding: 10px 20px;
            font-size: 18px;
            font-family: var(--font-family-button);
            background-color: var(--button-bg-color);
            margin: 5px;
            color: black;
            border: 1px solid black;
            width: 150px;
            height: 50px;
            display: inline-block;
            text-align: center;
            transition: background-color 0.3s, color 0.3s, box-shadow 0.3s;
        }

        .button:hover {
            background-color: var(--button-bg-color);
            color: var(--button-hover-color);
        }

        .button:active {
            background-color: var(--button-active-bg-color);
            color: var(--button-active-color);
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
        }

        .slidecontainer {
            width: 100%;
            margin: 20px auto;
        }

        .sliderHorizontal {
            -webkit-appearance: none;
            width: 80%;
            height: 25px;
            background: var(--slider-bg-color);
            outline: none;
            opacity: 1.0;
            -webkit-transition: .2s;
            transition: opacity .2s;
        }

        .sliderHorizontal:hover {
            opacity: 1;
        }

        .sliderHorizontal::-webkit-sliderHorizontal-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 25px;
            height: 25px;
            background: var(--slider-thumb-color);
            cursor: pointer;
            border-radius: 50%;
        }

        .sliderHorizontal::-moz-range-thumb {
            width: 25px;
            height: 25px;
            background: var(--slider-thumb-color);
            cursor: pointer;
            border-radius: 50%;
        }

        @media (max-width: 600px) {
            .button {
                width: 100px;
                height: 40px;
                font-size: 14px;
            }

            .sliderHorizontal {
                width: 90%;
                height: 20px;
            }
        }
    </style>
</head>
<body>
    <h1>El Gorki carro</h1>
    <button class="button" id="button1" onmousedown="toggle('A');" ontouchstart="toggle('A');" onmouseup="toggle('off');" ontouchend="toggle('off');">Forward Fast</button>
    <button class="button" id="button2" onmousedown="toggle('B');" ontouchstart="toggle('B');" onmouseup="toggle('off');" ontouchend="toggle('off');">Forward</button>
    <button class="button" id="button3" onmousedown="toggle('C');" ontouchstart="toggle('C');" onmouseup="toggle('off');" ontouchend="toggle('off');">Reverse</button>
    <button class="button" id="button4" onmousedown="toggle('D');" ontouchstart="toggle('D');" onmouseup="toggle('off');" ontouchend="toggle('off');">Backward</button>
    <div class="buttons">
        <div class="action_btn">
            <button class="button" id="button5" onmousedown="toggle('L');" ontouchstart="toggle('L');" onmouseup="toggle('FO');" ontouchend="toggle('FO');">Left</button>
            <button class="button" id="button6" onmousedown="toggle('R');" ontouchstart="toggle('R');" onmouseup="toggle('FO');" ontouchend="toggle('FO');">Right</button>
        </div>
    </div>
    <div class="slidecontainer">
        <input type="range" min="7" max="173" value="90" class="sliderHorizontal" id="myRange">
    </div>

    <div class="slidecontainer">
      <input type="range" min="7" max="173" value="90" class="sliderHorizontal" id="myRangeForward" style="transform: rotate(-90deg); display: none;">
    </div>


    <script>
        var sliderHorizontal = document.getElementById("myRange");
        sliderHorizontal.oninput = function() {
            pos = sliderHorizontal.value;
            steering(pos);
        }
        var sliderVertical = document.getElementById("myRangeForward");
        sliderVertical.oninput = function() {
            pos = sliderVertical.value;
            drive(pos);
        }
        function toggle(x) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/" + x, true);
            xhr.send();
        }

        function steering(pos) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/Pos?value=" + pos, true);
            xhr.send();
        }

        function drive(pos) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/Drive?value=" + pos, true);
            xhr.send();
        }
    </script>
</body>
</html>
)=====";