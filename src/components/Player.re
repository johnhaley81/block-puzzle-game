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

let playerIconStyles =
  Theme.(
    Css.(
      style([
        border(playerSize, Border.style, Colors.player),
        borderRadius(playerSize),
        height(`zero),
        width(`zero),
      ])
    )
  );

let component = ReasonReact.statelessComponent("Player");

let make = (~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles))>
      <div className=playerIconStyles />
    </div>,
};
