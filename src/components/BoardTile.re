module Component = {
  let component = ReasonReact.statelessComponent("Component");
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
      switch (boardPiece) {
      | Start => <StartTile />
      | End => <EndTile />
      | Block(id, move) => <BlockTile id move />
      | Player => <Player />
      | Empty => <div className onClick=(onClick(position)) />
      },
  };
};

let make = (~boardPiece, ~onClick, ~position, children) =>
  Styletron.React.makeStyledComponent(
    ~rule=
      _props => BsCssCore.Css.(style([flexGrow(1), flexBasis(rem(10.))])),
    ~component=Component.component,
    ~make=Component.make(~boardPiece, ~onClick, ~position),
    children,
  );
