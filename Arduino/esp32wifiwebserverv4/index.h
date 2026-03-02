
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
            margin: auto;
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



        @media (max-width: 600px) {
            .button {
                width: 100px;
                height: 40px;
                font-size: 14px;
            }
        }
    </style>
</head>
<body>
    <h1>El Gorki carro</h1>
    <div>
        <p>Event Count: <span id="event-count">0</span></p>
    </div>
    <div class="joystick-container">
        <div class="joystick"></div>
    </div>
    <button class="button" id="button1" onmousedown="toggle('FlashLight');" ontouchstart="toggle('FlashLight');" onmouseup="toggle('off');" ontouchend="toggle('off');">Flashlight</button>

    <script>
            const joystickContainer = document.querySelector('.joystick-container');
            const joystick = document.querySelector('.joystick');
            let containerRect, containerCenter, maxRadius;
            let count = 0;
            var prio = false;
            // const debouncedSendDrivingData = debouncedFunction(sendDrivingData, 100);
            const { throttleFn, immediateFn } = throttleFunction(sendDrivingData, 100);
            
            function debouncedFunction(callback, limit) {
                let waiting = false;
                return (...args) => {
                    if (!waiting) {
                        callback(...args);
                        waiting = true;
                        setTimeout(() => {
                            waiting = false;
                        }, limit);
                    }
                };
            }
            function throttleFunction(callback, limit) {
                let waiting = false;
                let waitingCallback;

                const throttleFn = (...args) => {
                    if (!waiting) {
                        callback(...args);
                        waiting = true;
                        waitingCallback = setTimeout(() => {
                            waiting = false;
                        }, limit);
                    }
                };

                // function immediateFn(...args) {
                //     clearTimeout(waitingCallback);
                //     callback(...args);
                // }
                const immediateFn = (...args) => {
                    clearTimeout(waitingCallback);
                    waiting = false;
                    callback(...args);
                }

                return {
                    throttleFn,
                    immediateFn,
                }
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

                // debouncedSendDrivingData(normalizedX, -normalizedY);
                throttleFn(normalizedX, -normalizedY);
                // debouncedSendDrivingData(Math.round(normalizedX * 255), Math.round(normalizedY * -255));
                // sendDrivingData(Math.round(normalizedX * 255), Math.round(normalizedY * -255));
            }

            function endDrag() {
                document.removeEventListener('mousemove', drag);
                document.removeEventListener('touchmove', drag);
                document.removeEventListener('mouseup', endDrag);
                document.removeEventListener('touchend', endDrag);
                resetJoystick();
                resetJoystick();
            }

            function resetJoystick() {
                joystick.style.left = '50%';
                joystick.style.top = '50%';
                joystick.style.transform = 'translate(-50%, -50%)'; // Center the joystick
                // debouncedSendDrivingData(0,0);
                immediateFn(0, 0);
                immediateFn(0, 0);
                immediateFn(0, 0);
                immediateFn(0, 0);

                // sendDrivingData(0,0);
            }

        function toggle(x) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/" + x, true);
            xhr.send();
        }

        function sendDrivingData(x, y) {
            var xhr = new XMLHttpRequest();
            count += 1; // Increment the variable
            document.getElementById('event-count').textContent = count;
            data = prepareData(x, y);
            xhr.open("GET", "/SentDrivingData?valueX=" + data.Posx + "&valueY=" + data.Posy + "&valueXn=" + data.Negx + "&valueYn=" + data.Negy, true);
            xhr.send();
            //console.log(x,y);
            //console.log(data);
        }

        function prepareData(x, y) {
            const max = 0.35;
            if (Math.abs(x) < max) {
                x = 0;
                var Posx = 0;
                var Negx = 0;
            }else{
                var Posx = x >= 0 ? Math.round((x-max)*255/(1-max)) : 0;
                var Negx = x >= 0 ? 0 : Math.round((-x-max)*255/(1-max));
            }
            if (Math.abs(y) < max) {
                y = 0;
                var Posy = 0;
                var Negy = 0;
            } else {
                var Posy = y >= 0 ? Math.round((y-max)*255/(1-max)) : 0;
                var Negy = y >= 0 ? 0 : Math.round((-y-max)*255/(1-max));
            }

            return { Posx, Posy, Negx, Negy };
        }
        

    </script>
</body>
</html>
)=====";