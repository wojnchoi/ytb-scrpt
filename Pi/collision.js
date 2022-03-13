let digits,
sBlock, bBlock;


const p5js = p => {
  let running_animation = false;
  let windwid = 600;
  let  initial_point, theta, global_time, frames_after_finished, speedo;
  /* https://codepen.io/kappeh/pen/bZMRgg */
  class Block {
    constructor(x,y,m,w) {
      this.x = x;
      this.y = y;
      this.mass = m;
      this.width = w;
    }
    //x 위치 업데이트
    updatePosition(x) {
      this.x = x;
    }

    //블럭 그리기
    display() {
      let posX = (this.mass > 1) ? (this.x- (p.textWidth(this.mass)) / 4) : this.x + 5;
      p.textSize(25);
      p.fill(24);
      p.text(this.mass, posX, this.y - 15);
      p.stroke(51);
      p.fill("#4c72e6");
      p.rect(this.x, this.y, this.width, this.width);
    }
  }

  class Point {
    constructor(x, y) {
      this.x = x;
      this.y = y;
    }
    
    rotate(angle) {
      let sin_angle = Math.sin(angle), cos_angle = Math.cos(angle);
      
      return new Point(
        this.x * cos_angle - this.y * sin_angle,
        this.x * sin_angle + this.y * cos_angle
      )
    }
    
    reflect(angle) {
      let sin_two_angle = Math.sin(2 * angle);
      let cos_two_angle = Math.cos(2 * angle);

      return new Point(
        this.x * cos_two_angle + this.y * sin_two_angle,
        this.x * sin_two_angle - this.y * cos_two_angle
      )
    }
    
    angle() {
      if (this.x >= 0)
        return Math.atan(this.y / this.x);
      else
        return Math.PI - Math.atan(-this.y / this.x);
    }
  }
  
  //시작 세팅
  function init_state(x1, m1, x2, m2) {
    
    let root_m1 = Math.sqrt(m1), root_m2 = Math.sqrt(m2);
    
    speedo = Math.sqrt(m2) * 2;
    
    initial_point = new Point(x2 * root_m2, x1 * root_m1);
    theta = Math.atan(root_m1 / root_m2);
    
  }

  function calculation_point(t) {
    return new Point(initial_point.x - speedo * t, initial_point.y);
  }
  
  function current_point(t) {
    
    let point = calculation_point(t);
    let phi = point.angle();
    let reflection_count = Math.floor(phi / theta);
    let rotation_angle = - theta * reflection_count;

    point = point.rotate(rotation_angle);
    if (reflection_count % 2 == 1) {
      point = point.reflect(theta / 2);
  
    }
    return point;
  }
  
  function going_to_collide(t) {
    let point = calculation_point(t);
    let phi = point.angle();
    
    let reflection_count = Math.ceil(phi / theta);
    let rotation_angle = theta * reflection_count;
    
    return (rotation_angle <= Math.PI);
  }
  
  function collision_count(t) {
    let point = calculation_point(t);
    let phi = point.angle();
    return Math.floor(phi / theta);
  }
  
  function current_state(t) {
    let point = current_point(t);
    
    point.x /=Math.sqrt(bBlock.mass);
    point.y /=Math.sqrt(sBlock.mass);
    
    return point;
  }
  
  function update_blocks(t) {
    let state = current_state(t);
    
    sBlock.x = state.y;
    bBlock.x = state.x + sBlock.width;
  }

  //결과 출력
  function draw_text(t) {
    if(tag == "col") {
      let outt = collision_count(t);
      let str = outt.toString();
      if(outt > 3)
        str = str.substring(0, 1) + "." + str.substring(1, str.length);
  
      document.getElementById("output-text").textContent = str
      document.getElementById("output-text").style.display = "block";
    }

  }

  p.setup = () => {
    //console.log(digits);
    p.createCanvas(windwid, 200);

    const w = 25;
    let w2 = (digits == 1) ? w : w * (3 - 2 * Math.exp(-0.1 * digits));
    
    sBlock = new Block(100, p.height - w-1, 1, w);
    bBlock = new Block(300, p.height - w2-1, Math.pow(100, digits-1), w2);


    init_state(100, 1, 300, p.pow(100, digits-1));

    global_time = 0;
    frames_after_finished = 1;
    
    if (running_animation == false) {
      running_animation = true;
    }	

  }

  p.draw = () => {
    p.frameRate(60)
    p.clear();
    //console.log(speedo + " " + global_time);
    update_blocks(global_time);
	
    sBlock.display();
    bBlock.display(); 
   
	  // Increasing the time step
	  global_time += 1;
    draw_text(global_time);
    // Continuing draw loop
    if (!going_to_collide(global_time)) {
      frames_after_finished -= 1;

    } 
    if (frames_after_finished < 0) { 
      running_animation = false;
  }

  }
}