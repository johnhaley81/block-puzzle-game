let _base = () => {
  let component = ReasonReact.statelessComponent("BoardTile");
  let make =
      (
        ~boardPiece: GameLogic.boardPiece,
        ~onClick,
        ~position,
        ~className,
        _children,
      ) => {
    ...component,
    render: _self =>
      <div className onClick=(onClick(position))>
        (
          switch (boardPiece) {
          | Start => <StartTile />
          | End => <EndTile />
          | Block(id, move) => <BlockTile id move />
          | Player => <Player />
          | Empty => ReasonReact.null
          }
        )
      </div>,
  };
  (component, make);
};

let make = (~boardPiece, ~onClick, ~position, ~totalColumns, children) =>
  _base()
  |> (
    ((component, make)) =>
      Styletron.React.makeStyledComponent(
        ~rule=_props => BsCssCore.Css.(style([flex(1)])),
        ~component,
        ~make=make(~boardPiece, ~onClick, ~position),
        children,
      )
  );
