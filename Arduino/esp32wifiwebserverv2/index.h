const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <title>El Gorki carro</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <style>
        body {
            display: block;
            justify-content: center;
            align-items: center;
            height: 100vh;
            width: 100%;
            background-color: var(--main-bg-color);
            font-family: var(--font-family);
            user-select: none;
            text-align: center;
            margin: 0;
            padding-top: 30px;

        }

        .joystick-container {
            position: relative;
            width: 200px;
            height: 200px;
            background-color: #fff;
            border-radius: 50%;
            border: 2px solid #000;
            display: flex;
            justify-content: center;
            align-items: center;
            margin-bottom: 20px 0;
        }

        .joystick {
            position: absolute;
            width: 50px;
            height: 50px;
            background-color: #000;
            border-radius: 50%;
        }

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
            width: 40%;
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
    <div class="joystick-container">
        <div class="joystick"></div>
    </div>
    <button class="button" id="button1" onmousedown="toggle('FlashLight');" ontouchstart="toggle('FlashLight');" onmouseup="toggle('off');" ontouchend="toggle('off');">Flashlight</button>

    <!-- <div class="slidecontainer">
        <input type="range" min="7" max="173" value="90" class="sliderHorizontal" id="myRange">
    </div>

    <div class="slidecontainer">
      <input type="range" min="7" max="173" value="90" class="sliderHorizontal" id="myRangeForward" style="transform: rotate(-90deg); display: none;">
    </div> -->

    <script>
            const joystickContainer = document.querySelector('.joystick-container');
            const joystick = document.querySelector('.joystick');
            let containerRect, containerCenter, maxRadius;

            function debounce(callback, wait) {
                let timerId;
                return (...args) => {
                    clearTimeout(timerId);
                    timerId = setTimeout(() => callback(...args), wait);
                };
            }

            function initializeJoystick() {
                containerRect = joystickContainer.getBoundingClientRect();
                containerCenter = {
                    x: containerRect.width / 2,
                    y: containerRect.height / 2
                };
                maxRadius = containerRect.width / 2 - joystick.offsetWidth / 2;
                resetJoystick(); // Ensure the joystick starts centered
            }

            initializeJoystick();
            window.addEventListener('resize', initializeJoystick);

            joystick.addEventListener('mousedown', startDrag);
            joystick.addEventListener('touchstart', startDrag);

            function startDrag(event) {
                event.preventDefault();
                document.addEventListener('mousemove', drag);
                document.addEventListener('touchmove', drag);
                document.addEventListener('mouseup', endDrag);
                document.addEventListener('touchend', endDrag);
            }

            function drag(event) {
                event.preventDefault();
                let clientX = event.clientX || event.touches[0].clientX;
                let clientY = event.clientY || event.touches[0].clientY;

                const deltaX = clientX - containerRect.left - containerCenter.x;
                const deltaY = clientY - containerRect.top - containerCenter.y;
                const angle = Math.atan2(deltaY, deltaX);
                const distance = Math.min(maxRadius, Math.sqrt(deltaX ** 2 + deltaY ** 2));

                const x = containerCenter.x + distance * Math.cos(angle);
                const y = containerCenter.y + distance * Math.sin(angle);

                joystick.style.left = `${x - joystick.offsetWidth / 2}px`;
                joystick.style.top = `${y - joystick.offsetHeight / 2}px`;
                joystick.style.transform = ''; // Clear transform during drag

                const normalizedX = (x - containerCenter.x) / maxRadius;
                const normalizedY = (y - containerCenter.y) / maxRadius;

                steering(Math.round(normalizedX * 100));
                drive(Math.round(normalizedY * 100));
            }

            function endDrag() {
                document.removeEventListener('mousemove', drag);
                document.removeEventListener('touchmove', drag);
                document.removeEventListener('mouseup', endDrag);
                document.removeEventListener('touchend', endDrag);
                resetJoystick();
            }

            function resetJoystick() {
                joystick.style.left = '50%';
                joystick.style.top = '50%';
                joystick.style.transform = 'translate(-50%, -50%)'; // Center the joystick
                steering(0);
                drive(0);
            }


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
            debounce(() => {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/Pos?value=" + pos, true);
                xhr.send();
            }, 50);
            
        }

        function drive(pos) {
            debounce(() => {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/Drive?value=" + pos, true);
                xhr.send();
            }, 50);
        }
    </script>
</body>
</html>
)=====";