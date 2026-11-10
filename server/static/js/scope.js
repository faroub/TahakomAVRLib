class ScopeViewer {
    constructor(canvasId, isLogic = false) {
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d');
        this.data = new Array(100).fill(0);
        this.isLogic = isLogic;
        this.decodedFrames = [];
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

    addDecodedFrame(str) {
        this.decodedFrames.push({ time: Date.now(), label: str });
        if (this.decodedFrames.length > 5) this.decodedFrames.shift();
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
        this.ctx.strokeStyle = this.isLogic ? '#55efc4' : '#00d2ff';
        this.ctx.lineWidth = 2;
        this.ctx.beginPath();
        const step = w / (this.data.length - 1);
        for (let i = 0; i < this.data.length; i++) {
            const x = i * step;
            const norm = this.isLogic ? (this.data[i] ? 0.8 : 0.2) : (this.data[i] / 1023);
            const y = h - (norm * (h - 20) + 10);
            if (i === 0) this.ctx.moveTo(x, y);
            else {
                if (this.isLogic) {
                    const prevX = (i - 1) * step;
                    this.ctx.lineTo(x, h - ((this.data[i - 1] ? 0.8 : 0.2) * (h - 20) + 10));
                }
                this.ctx.lineTo(x, y);
            }
        }
        this.ctx.stroke();

        // Protocol Decoder overlay
        if (this.isLogic && this.decodedFrames.length > 0) {
            this.ctx.fillStyle = '#ff7675';
            this.ctx.font = '11px monospace';
            const lastFrame = this.decodedFrames[this.decodedFrames.length - 1];
            this.ctx.fillText(`Decoded Frame: ${lastFrame.label}`, 10, 20);
        }
    }
}

const scope = new ScopeViewer('scopeCanvas', false);
const logicScope = new ScopeViewer('logicCanvas', true);
