let baseStyles = Styles.makeStyles(Css.[flexGrow(1)]);

let component = ReasonReact.statelessComponent("EndTile");

let make = (~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles))> ("E" |> ReasonReact.string) </div>,
};
