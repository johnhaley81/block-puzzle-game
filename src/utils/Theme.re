module Border = {
  let color = Css.hex("215A87");
  let size = `em(0.0625);
  let startAndEndBorderSize = `em(1.0 /. 3.0);
  let style = `solid;
  let boardRadius = `em(1.0);
  let boardTopBottomSize = `em(2.25);
  let boardLeftRightSize = `em(1.5);
};

module Colors = {
  let board = Css.hex("19476C");
  let blockBase = Css.hex("0E293E");
  let blockMovePointer = Css.hex("AFC5D5");
  let player = Css.hex("FFFFFF");
  let startEndTileHole = Css.hex("000000");
};

let baseTileSizeMultiplier = 2.625;

let playerSize = `em(0.75);
