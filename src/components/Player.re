let baseStyles = Styles.makeStyles(Css.[flexGrow(1)]);

let component = ReasonReact.statelessComponent("Player");

let make = (~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(baseStyles(styles))> ("P" |> ReasonReact.string) </div>,
};
