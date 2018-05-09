let baseStyles = Styles.makeStyles(Css.[flexGrow(1)]);

let component = ReasonReact.statelessComponent("StartTile");

let make = (~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles))> ("S" |> ReasonReact.string) </div>,
};
