let makeStyles =
  Theme.(
    Styles.makeStyles(
      Css.[
        borderBottom(Border.size, Border.style, Border.color),
        lastChild([borderBottomColor(`transparent)]),
        display(`flex),
        flex(1),
        alignItems(`center),
      ],
    )
  );

let component = ReasonReact.statelessComponent("BoardRow");

let make = (~onTileClick, ~rowNumber, ~tiles, ~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(makeStyles(styles))>
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
