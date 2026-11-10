class ScopeViewer {
    constructor(canvasId) {
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d');
        this.data = new Array(100).fill(0);
        this.resize();
        window.addEventListener('resize', () => this.resize());
        this.draw();
    }

    resize() {
        this.canvas.width = this.canvas.clientWidth;
        this.canvas.height = this.canvas.clientHeight;
    }

    pushSample(val) {
        this.data.shift();
        this.data.push(val);
        this.draw();
    }

    draw() {
        const w = this.canvas.width;
        const h = this.canvas.height;
        this.ctx.fillStyle = '#0d0d14';
        this.ctx.fillRect(0, 0, w, h);

        // Grid lines
        this.ctx.strokeStyle = '#1e1e2d';
        this.ctx.lineWidth = 1;
        for (let x = 0; x < w; x += w / 10) {
            this.ctx.beginPath();
            this.ctx.moveTo(x, 0);
            this.ctx.lineTo(x, h);
            this.ctx.stroke();
        }
        for (let y = 0; y < h; y += h / 4) {
            this.ctx.beginPath();
            this.ctx.moveTo(0, y);
            this.ctx.lineTo(w, y);
            this.ctx.stroke();
        }

        // Plot line
        this.ctx.strokeStyle = '#00d2ff';
        this.ctx.lineWidth = 2;
        this.ctx.beginPath();
        const step = w / (this.data.length - 1);
        for (let i = 0; i < this.data.length; i++) {
            const x = i * step;
            const norm = this.data[i] / 1023; // 10-bit ADC
            const y = h - (norm * (h - 10) + 5);
            if (i === 0) this.ctx.moveTo(x, y);
            else this.ctx.lineTo(x, y);
        }
        this.ctx.stroke();
    }
}

const scope = new ScopeViewer('scopeCanvas');
const logicScope = new ScopeViewer('logicCanvas');
