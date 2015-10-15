/**
 * @name Harmonoscope
 * @description <p>Visualize the frequency live audio input tuned to A4.</p>
 * <p><em><span class="small"> To run this example locally, you will need the
 * <a href="http://p5js.org/reference/#/libraries/p5.sound">p5.sound library</a>
 * and a running <a href="https://github.com/processing/p5.js/wiki/Local-server">local server</a>.</span></em></p>
 */
var mic, fft;
var debug = true;

allTheNotes = [{
  key: "C2",
  freq: 65.406
}, {
  key: "Csharp2",
  freq: 69.296
}, {
  key: "D2",
  freq: 73.416
}, {
  key: "Dsharp2",
  freq: 77.782
}, {
  key: "E2",
  freq: 82.407
}, {
  key: "F2",
  freq: 87.307
}, {
  key: "Fsharp2",
  freq: 92.499
}, {
  key: "G2",
  freq: 97.999
}, {
  key: "Gsharp2",
  freq: 103.826
}, {
  key: "A2",
  freq: 110.00
}, {
  key: "Asharp2",
  freq: 116.541
}, {
  key: "B2",
  freq: 123.471
}, {
  key: "C3",
  freq: 130.813
}, {
  key: "Csharp3",
  freq: 138.591
}, {
  key: "D3",
  freq: 146.832
}, {
  key: "Dsharp3",
  freq: 155.563
}, {
  key: "E3",
  freq: 164.814
}, {
  key: "F3",
  freq: 174.614
}, {
  key: "Fsharp3",
  freq: 184.997
}, {
  key: "G3",
  freq: 195.998
}, {
  key: "Gsharp3",
  freq: 207.652
}, {
  key: "A3",
  freq: 220.00
}, {
  key: "Asharp3",
  freq: 233.082
}, {
  key: "B3",
  freq: 246.942
}, {
  key: "C4",
  freq: 261.626
}, {
  key: "Csharp4",
  freq: 277.183
}, {
  key: "D4",
  freq: 293.665
}, {
  key: "Dsharp4",
  freq: 311.127
}, {
  key: "E4",
  freq: 329.628
}, {
  key: "F4",
  freq: 349.228
}, {
  key: "Fsharp4",
  freq: 369.994
}, {
  key: "G4",
  freq: 391.995
}, {
  key: "Gsharp4",
  freq: 415.305
}, {
  key: "A4",
  freq: 440.00
}, {
  key: "Asharp4",
  freq: 466.164
}, {
  key: "B4",
  freq: 493.883
}, {
  key: "C5",
  freq: 523.251
}, {
  key: "Csharp5",
  freq: 554.365
}, {
  key: "D5",
  freq: 587.33
}, {
  key: "Dsharp5",
  freq: 622.254
}, {
  key: "E5",
  freq: 659.255
}, {
  key: "F5",
  freq: 698.456
}, {
  key: "Fsharp5",
  freq: 739.989
}, {
  key: "G5",
  freq: 783.991
}, {
  key: "Gsharp5",
  freq: 830.609
}, {
  key: "A5",
  freq: 880.00
}, {
  key: "Asharp5",
  freq: 932.328
}, {
  key: "B5",
  freq: 987.767
}];

var currentKeyIdx = 36;
var freq = allTheNotes[currentKeyIdx].freq;

function setup() {
  createCanvas(600, 600);
  noFill();

  mic = new p5.AudioIn();
  mic.start();
  fft = new p5.FFT();
  fft.setInput(mic);
}

function draw() {
  background(200);

  if (!debug) {
    fft.analyze();

    // Find note with maximum energy at that frequency
    var maxVal = -Infinity;
    currentKeyIdx = -1;
    for (idx in allTheNotes) {
      energy = fft.getEnergy(allTheNotes[idx].freq);
      if (energy > maxVal) {
        maxVal = energy;
        currentKeyIdx = idx;
      }
    }
    console.log(allTheNotes[currentKeyIdx].key);
  }

  // Smoothly interpolate from current fequency to target
  freq = interpolate(allTheNotes[currentKeyIdx].freq, freq);

  // Perturb the frequencies slightly so the shape looks "alive"
  freqEps = perturb(freq);
  freq2Eps = perturb(allTheNotes[21].freq);

  // Draw the shape
  drawFigure(freqEps, freq2Eps);
}

function interpolate(targetFrequency, currentFrequency) {
  interpCoeff = .9;
  currentFrequency = interpCoeff * currentFrequency + (1 - interpCoeff) * targetFrequency;
  return currentFrequency;
}

function perturb(frequency) {
  range = .05
  return frequency + random(-range, range);
}

function drawFigure(freq1, freq2) {
  dt = .005;
  decay = -.05;

  beginShape();
  for (var time = PI * PI / freq1; time < TWO_PI + PI * PI / freq1; time += dt) {
    x = map(sin(freq1 / TWO_PI * time) * exp(decay * time), -1, 1, 0, width);
    y = map(sin(freq2 / TWO_PI * time) * exp(decay * time), -1, 1, 0, height);
    curveVertex(x, y);
  }
  endShape();
}


function keyPressed() {
  if (debug) {
    if (keyCode == UP_ARROW) {
      currentKeyIdx = (currentKeyIdx + 1) % allTheNotes.length;
    } else if (keyCode = DOWN_ARROW) {
      currentKeyIdx = (currentKeyIdx - 1 + allTheNotes.length) % allTheNotes.length;
    }

    console.log(allTheNotes[currentKeyIdx].key)
  }
}