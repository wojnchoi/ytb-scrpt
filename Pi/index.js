let tag = "none";
let looping = false;

function Update(num) {
    document.getElementById("logo").style.display = "none";
    if(num == 1) {
        tag = "col";
        document.getElementById("outs").innerHTML = `
        <h3 id="output-text" style="font-family: Helvetica; font-size:30px;"></h3>
        <p id="displaydigit">자리수: </p>
        <input id="digitcount" onchange="collision()" type="range" min="1" max="15" value="10">
        `;
        collision();
    } else if(num == 2) {
        tag = "rand";
        document.getElementById("outs").innerHTML = `
        <h3 id="output-text" style="font-family: Helvetica; font-size:30px;"></h3>
        <div class="slidecontainer">
          <p id="n-sides"></p>
          <input id="sides-sliderr" onchange="randomNumbers()" type="range" min="0" max="100000" value="0">
        </div>
        `;
        randomNumbers();

    } else if(num == 3) {
        tag = "archi";
        document.getElementById("outs").innerHTML = `
        <h3 id="output-text" style="font-family: Helvetica; font-size:30px;"></h3>
        <div class="slidecontainer">
          <p id="n-sides">면:</p>
          <input type="text" onchange="archi(1)" id="input-sides" value="">
          <input id="sides-slider" onchange="archi(0)" type="range" min="3" max="15" value="3">
        </div>
        `;
        archi(0);

    }
}

function collision() {

    var view = document.getElementById("show-content");
        view.innerHTML = `
        <h2>충돌</h2>
        `;

    digits = document.getElementById("digitcount").value;
    document.getElementById("displaydigit").innerText = "자릿수: " + digits;

    new p5(p5js, document.getElementById("show-content")); //p5js를 이용한 충돌 애니메이션

}



function randomNumbers() {
    var output = document.getElementById("output-text");
    var view = document.getElementById("show-content");
    view.innerHTML = `
    <h2>난수</h2>
    <canvas id="circle-canvas" width="600" height="600"></canvas>
    `;

    const canvas = document.getElementById('circle-canvas');
    const context = canvas.getContext('2d');

    const X = canvas.width/2;
    const Y = canvas.height/2;
    const R = canvas.width/2;
    context.arc(X,Y,R,0,2*Math.PI);
    context.strokeStyle = "Blue";
    context.lineWidth = 2;
    context.stroke();
    
    context.beginPath();
    context.moveTo(X, Y);
    context.lineTo(X*2, Y);
    context.moveTo(X, Y);
    context.font = "15px Arial";
    context.fillText("1", 3*X/2, Y-5);
    context.stroke();
    let pointTotal = document.getElementById("sides-sliderr").value; 
    let pointInCircle = randomPoints(context, pointTotal, X, Y, R);
    output.textContent = (pointTotal != 0) ?4*pointInCircle/pointTotal : "오늘 저녁은 애플π?"; //4*원 안에있는 점 나누기 n

    //console.log(4*pointInCircle/pointTotal);
    document.getElementById("n-sides").innerText = "n: " + pointTotal;
}


//난수 그리기
function randomPoints(canvas, count, X, Y, R){
    let ret = 0;
    for (var i = 0; i < count; i++) {
        var x = (Math.random() * X*2);
        var y = (Math.random() * Y*2);
        if(Math.sqrt(Math.pow(x-X, 2)+Math.pow(y-Y, 2)) > R)
            canvas.fillStyle = "Green";
        else {
            canvas.fillStyle = "Red";
            ret++;
        }

        canvas.fillRect(x,y,3,3);
    }
    return ret;
  }

function archi(num) {
    var output = document.getElementById("output-text");
    var view = document.getElementById("show-content");

    view.innerHTML = `
        <h2>아르키메데스</h2>
        <canvas id="circle-canvas" width="1000" height="700"></canvas>
    `;
    
    var n_text = document.getElementById("n-sides");
    const R = 200; //R 기준점

    n_text.innerHTML = "면: ";
    
    let sides_value;
    let sides_text = document.getElementById("input-sides");
    
    if(!num) {
        sides_value = document.getElementById("sides-slider").value;
        sides_text.value = sides_value;
    } else {
        sides_value = sides_text.value;
        document.getElementById("sides-slider").value = sides_value;
    }
    
    //let side_length = 2*R*(Math.sin(Math.PI/sides_value));
    //console.log(sides_value);
    //console.log(side_length);

    const canvas = document.getElementById('circle-canvas');
    const context = canvas.getContext('2d');
    
    const X = canvas.width/2;
    const Y = canvas.height/2 +60;
    context.beginPath();
    context.arc(X, Y, R/* 잘못계산함 ㅋㅋ R*Math.cos(Math.PI/sides_value)*/, 0, 2 * Math.PI); //원
    context.strokeStyle = "Blue";
    context.lineWidth = 2;
    context.stroke();

    context.beginPath();
    draw_polygon(context, X,Y,R*1/Math.cos(Math.PI/sides_value)/* 이것도 side_length/(2*Math.sin(180/sides_value*Math.PI/180))*/,sides_value, 1,1); /* 바깥 도형 (외접 circumscribed) */
    context.strokeStyle = "Black";
    context.lineWidth = 1;
    context.stroke();

    context.beginPath();
    draw_polygon(context, 0,0,R/* 이것도.. R*Math.cos(Math.PI/sides_value)*/,sides_value, 1,1); /* 안쪽 도형 (내접 inscribed) */
    context.strokeStyle = "Black";
    context.lineWidth = 1;
    context.stroke();


    var insc_side = 2*R*Math.sin(Math.PI/sides_value);
    //console.log(insc_side*sides_value);
    var circ_side = 2*R*(Math.tan(Math.PI/sides_value))
    //console.log(circ_side*sides_value);

    var insc_param = insc_side * sides_value / R;
    var circ_param = circ_side * sides_value / R;

    //console.log(insc_param/2 + " " + circ_param/2);

    /* 아르키메데스 센세가 알려준데로 L1/2 < pi < L2/2 */
    output.innerHTML = insc_param/2 + "< π <" + circ_param/2;
}

//모양 그려주는 함수
function draw_polygon(ctx, x, y, radius, sides) {
    if (sides < 3) return;
    var a = (Math.PI * 2)/sides;
    
    ctx.translate(x,y);
    ctx.rotate(-Math.PI/2);
    ctx.moveTo(radius,0);
    for (var i = 1; i < sides; i++) {
      ctx.lineTo(radius*Math.cos(a*i),radius*Math.sin(a*i));

    }
    ctx.closePath();
  }