let _base = () => {
  let component = ReasonReact.statelessComponent("BoardRow");
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
                 totalColumns=(tiles |> Belt.List.length)
               />
             )
          |> Belt.List.toArray
          |> ReasonReact.array
        )
      </div>,
  };
  (component, make);
};

let make = (~onTileClick, ~rowNumber, ~tiles, ~totalRows, children) =>
  _base()
  |> (
    ((component, make)) =>
      Styletron.React.makeStyledComponent(
        ~rule=_props => BsCssCore.Css.(style([display(Flex), flex(1)])),
        ~component,
        ~make=make(~onTileClick, ~rowNumber, ~tiles),
        children,
      )
  );
