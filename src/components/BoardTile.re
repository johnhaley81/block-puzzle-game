let baseStyles =
  Styles.makeStyles(
    Theme.(
      Css.[
        borderRight(Border.size, Border.style, Border.color),
        lastChild([borderRightColor(`transparent)]),
        display(`flex),
        flex(1),
        height(`em(baseTileSizeMultiplier)),
        width(`em(baseTileSizeMultiplier)),
        boxShadow(~inset=true, ~blur=`em(0.0625), `rgba((0, 0, 0, 0.4))),
      ]
    ),
  );

let component = ReasonReact.statelessComponent("BoardTile");

let make =
    (
      ~boardPiece: GameLogic.boardPiece,
      ~onClick,
      ~position,
      ~styles=[],
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles)) onClick=(onClick(position))>
      (
        switch (boardPiece) {
        | Start => <StartTile />
        | End => <EndTile />
        | Block(id, move) => <BlockTile id move />
        | Player => <Player />
        | Empty => <div />
        }
      )
    </div>,
};
