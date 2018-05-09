let makeStylesFromProps = (baseStyles, additionalStyles, props) =>
  Css.([additionalStyles(props), baseStyles(props)] |> Css.merge |> style);

let makeStyles = (baseStyles, additionalStyles) =>
  makeStylesFromProps((_) => baseStyles, (_) => additionalStyles, ());
