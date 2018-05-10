let baseStyles = Styles.makeStyles(Css.[flexGrow(1)]);

let component = ReasonReact.statelessComponent("EndTile");

let make = (~containsPlayer, ~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles))>
      (containsPlayer ? <Player /> : "E" |> ReasonReact.string)
    </div>,
};
