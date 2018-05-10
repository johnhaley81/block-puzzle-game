let baseStyles = Styles.makeStyles(Css.[flexGrow(1)]);

let component = ReasonReact.statelessComponent("StartTile");

let make = (~containsPlayer, ~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles))>
      (containsPlayer ? <Player /> : "S" |> ReasonReact.string)
    </div>,
};
