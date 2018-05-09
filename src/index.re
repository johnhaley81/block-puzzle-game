Css.(global("html", [boxSizing(`borderBox), height(pct(100.0))]));

Css.(
  global(
    "body",
    [
      display(`flex),
      alignItems(`center),
      justifyContent(`center),
      margin(`zero),
      padding(`zero),
      height(pct(100.0)),
      fontFamily("'PT Sans', sans-serif"),
      color(gray),
      lineHeight(1.3),
      textTransform(`lowercase),
    ],
  )
);

ReactDOMRe.renderToElementWithId(<Router />, "index");
