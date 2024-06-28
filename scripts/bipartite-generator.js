const fs = require('node:fs');

(function() {
  'use strict';
  const H = [];
  const S = [];
  
  for (let i = 0; i < 30; ++i) {
    H.push(`H${i.toString().padStart(2,"0")}`);
    S.push(`s${i.toString().padStart(2,"0")}`);
  }
  const Hsh = [...H];
  const Ssh = [...S];

  let text = "";
  
  for (const h of H) {
    shuffle(Ssh);
    text += `${h}: ${Ssh.join(",")};\n`;
  }
  text += `***;\n`
  for (const s of S) {
    shuffle(Hsh);
    text += `${s}: ${Hsh.join(",")};\n`;
  }

  fs.writeFile('./test-cases/input-001.txt', text, err => {
    if (err) {
      console.error(err);
    } else {
      console.error("File was generated");
    }
  });

  function getRandInt(max) {
    return Math.floor(Math.random() * max);
  }

  function shuffle(arr) {
    for (const it of arr) {
      const [i, j] = [getRandInt(arr.length), getRandInt(arr.length)];
      [arr[i], arr[j]] = [arr[j], arr[i]];
    }
  }
}());