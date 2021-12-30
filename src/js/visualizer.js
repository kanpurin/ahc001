let canvas = document.getElementById('myCanvas');
let ctx = canvas.getContext('2d');
let play;
let cursorX = 0, cursorY = 0;
const fontsize = 150;

ctx.strokeStyle = "black";
ctx.lineWidth = 15;

function drawSircle(x, y, r, color) {
    ctx.beginPath();
    ctx.arc(x, y, r, 0, Math.PI * 2);
    ctx.fillStyle = color;
    ctx.fill();
    ctx.closePath();
}

function drawLine(x1,y1,x2,y2) {
    ctx.beginPath();
    ctx.moveTo(x1,y1);
    ctx.lineTo(x2,y2);
    ctx.closePath();
    ctx.stroke();
}

function drawRect(x1,y1,x2,y2,color) {
    ctx.beginPath();
    ctx.rect(x1, y1, x2-x1, y2-y1);
    ctx.fillStyle = color;
    ctx.fill();
    ctx.closePath();
    
    ctx.strokeRect(x1,y1,x2-x1,y2-y1);
}

function isOverlap1D(x1,y1,x2,y2) {
    return x1 < y2 && x2 < y1;
}

function isOverlap2D(rect1, rect2) {
    return isOverlap1D(rect1[0],rect1[2],rect2[0],rect2[2]) && isOverlap1D(rect1[1],rect1[3],rect2[1],rect2[3]);
}

function getScore(turn) {
    if (!isLoadInput || !isLoadLog) return -1;
    for (let i = 0; i < N; i++) {
        if (advLog[turn][i][0] < 0 || advLog[turn][i][1] < 0 || advLog[turn][i][2] > 10000 || advLog[turn][i][3] > 10000) {
            console.log("Out of range : " + String(i));
            return -1;
        }
    }
    for (let i = 0; i < N-1; i++) {
        for (let j = i+1; j < N; j++) {
            if (isOverlap2D(advLog[turn][i],advLog[turn][j])) {
                console.log("Overlap : " + String(i) + " and " + String(j));
                return -1;
            }
        }
    }
    let score = 0;
    for (let i = 0; i < N; i++) {
        let S = (advLog[turn][i][2]-advLog[turn][i][0])*(advLog[turn][i][3]-advLog[turn][i][1]);
        if (advLog[turn][i][0] <= advX[i] && advLog[turn][i][1] <= advY[i] && advX[i] < advLog[turn][i][2] && advY[i] < advLog[turn][i][3]) {
            score += 1-(1-Math.min(advR[i],S)/Math.max(advR[i],S))**2;
        }
    }
    return Math.round(1e9*score/N);
}

function draw() {
    document.getElementById('score').value = getScore(document.getElementById("slider").value-1);
    if (canvas.getContext) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        if (isLoadLog) {
            for (let i = 0; i < N; i++) {
                let turn = document.getElementById("slider").value-1;
                let advRect = advLog[turn][i];
                let S = (advRect[2]-advRect[0])*(advRect[3]-advRect[1]);
                if (advR[i] >= S) {
                    drawRect(advRect[0],advRect[1],advRect[2],advRect[3],"rgb("+String(Math.floor(255*Math.min(advR[i],S)/Math.max(advR[i],S)))+",0,255)");
                }
                else {
                    drawRect(advRect[0],advRect[1],advRect[2],advRect[3],"rgb(255,0,"+String(Math.floor(255*Math.min(advR[i],S)/Math.max(advR[i],S)))+")");
                }
            }
        }
        if (isLoadInput) {
            for (let i = 0; i < N; i++) {
                drawRect(advX[i]-10,advY[i]-10,advX[i]+10,advY[i]+10, "#00DD33");
            }
        }
        if (isLoadInput && isLoadLog) {
            ctx.font = String(fontsize)+'pt Arial';
            ctx.fillStyle = "black";
            for (let i = 0; i < N; i++) {
                let turn = document.getElementById("slider").value-1;
                let advRect = advLog[turn][i];
                let S = (advRect[2]-advRect[0])*(advRect[3]-advRect[1]);
                drawLine(advX[i],advY[i],(advRect[0]+advRect[2])/2,(advRect[1]+advRect[3])/2);
                ctx.fillText(String(i)+":"+String(Math.floor(Math.min(advR[i],S)/Math.max(advR[i],S)*100)/100), (advRect[0]+advRect[2])/2, (advRect[1]+advRect[3])/2+fontsize/2);
            }
        }
        // drawSircle(cursorX, cursorY, 10, '#222');
    }
}

function increaseSlider() {
    document.getElementById('slider').value++;
    if (Number(document.getElementById('slider').value) >=
        Number(document.getElementById('slider').max)) {
        document.getElementById('slider').value = document.getElementById('slider').min;
    }
    document.getElementById('output1').value = document.getElementById('slider').value+"/"+String(T);
}

function autoPlay() {
    increaseSlider();
    draw();
}