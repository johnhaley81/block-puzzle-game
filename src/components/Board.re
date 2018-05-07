module Component = {
  let component = ReasonReact.statelessComponent("Component");
  let make = (~board: GameLogic.Board.t, ~onTileClick, ~className, _children) => {
    ...component,
    render: _self =>
      <div className>
        (
          board
          |. Belt.List.mapWithIndex((rowNumber, tiles) =>
               <BoardRow
                 onTileClick
                 tiles
                 rowNumber
                 key=(rowNumber |> string_of_int)
               />
             )
          |> Belt.List.toArray
          |> ReasonReact.array
        )
      </div>,
  };
};

let make = (~board, ~onTileClick, children) =>
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
    ~make=Component.make(~board, ~onTileClick),
    children,
  );
