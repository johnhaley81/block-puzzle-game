let _base = () => {
  let component = ReasonReact.statelessComponent("Board");
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
                 totalRows=(board |> Belt.List.length)
                 key=(rowNumber |> string_of_int)
               />
             )
          |> Belt.List.toArray
          |> ReasonReact.array
        )
      </div>,
  };
  (component, make);
};

let make = (~board, ~onTileClick, children) =>
  _base()
  |> (
    ((component, make)) =>
      Styletron.React.makeStyledComponent(
        ~rule=
          _props =>
            BsCssCore.Css.(
              style([
                display(Flex),
                flexDirection(Column),
                flexWrap(Wrap),
                flexGrow(1),
              ])
            ),
        ~component,
        ~make=make(~board, ~onTileClick),
        children,
      )
  );
