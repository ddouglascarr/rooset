import {h} from 'preact';
import {lazy, Suspense} from 'preact/compat';
import {EditorProps} from './Editor';

const EditorLoaded = lazy(() => import('./Editor'));

// TODO: any
export const Editor = (props: EditorProps) => (
  <Suspense fallback={<div>Loading ...</div>}>
    <EditorLoaded {...props} />
  </Suspense>
)
