import { h } from "preact";
import "tinymce/tinymce";

import "tinymce/icons/default";
import "tinymce/themes/silver";
import "tinymce/plugins/lists";

import { Editor as TinyMCEEditor } from "@tinymce/tinymce-react";

export type EditorProps = {
  initialValue: string;
  onEditorChange: (content: string) => void;
};

export const EditorLoaded = (props: EditorProps) => (
  <TinyMCEEditor
    initialValue={props.initialValue}
    onEditorChange={props.onEditorChange}
    init={{
      contextmenu: false,
      elementpath: false,
      branding: false,
      menubar: false,
      plugins: "lists",
      toolbar: "undo redo h3 bullist numlist",
      skin_url: "/static/js/tinymce/skins/ui/oxide/",
    }}
  />
);

export { EditorLoaded as default };
