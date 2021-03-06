let baseStyles =
  Styles.makeStyles(
    Css.[
      flexGrow(1),
      height(`percent(100.0)),
      display(`flex),
      alignItems(`center),
      justifyContent(`center),
    ],
  );

let startTileStyles =
  Theme.(
    Css.(
      style([
        backgroundColor(Colors.startEndTileHole),
        border(Border.startAndEndBorderSize, Border.style, Border.color),
        borderRadius(`calc((`add, playerSize, playerSize))),
        height(`calc((`add, playerSize, playerSize))),
        width(`calc((`add, playerSize, playerSize))),
      ])
    )
  );

let component = ReasonReact.statelessComponent("StartTile");

let make = (~containsPlayer, ~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles))>
      <div className=startTileStyles>
        (containsPlayer ? <Player /> : ReasonReact.null)
      </div>
    </div>,
};
