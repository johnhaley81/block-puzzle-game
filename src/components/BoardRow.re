module Component = {
  let component = ReasonReact.statelessComponent("Component");
  let make = (~onTileClick, ~rowNumber, ~tiles, ~className, _children) => {
    ...component,
    render: _self =>
      <div className>
        (
          tiles
          |. Belt.List.mapWithIndex((col, boardPiece) =>
               <BoardTile
                 boardPiece
                 key=(col |> string_of_int)
                 onClick=onTileClick
                 position=(GameLogic.Position.make(rowNumber, col))
               />
             )
          |> Belt.List.toArray
          |> ReasonReact.array
        )
      </div>,
  };
};

let make = (~onTileClick, ~rowNumber, ~tiles, children) =>
  Styletron.React.makeStyledComponent(
    ~rule=
      _props =>
        BsCssCore.Css.(
          style([
            display(Flex),
            flexWrap(Wrap),
            flexGrow(1),
            flexBasis(rem(10.)),
          ])
        ),
    ~component=Component.component,
    ~make=Component.make(~onTileClick, ~rowNumber, ~tiles),
    children,
  );
